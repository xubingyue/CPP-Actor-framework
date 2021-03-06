#include "bind_qt_run.h"

#ifdef ENABLE_QT_ACTOR
bind_qt_run_base::ui_tls::ui_tls()
:_uiStack(64)
{
	_count = 0;
	memset(_tlsBuff, 0, sizeof(_tlsBuff));
}

bind_qt_run_base::ui_tls::~ui_tls()
{
	assert(0 == _count);
}

void bind_qt_run_base::ui_tls::init()
{
	void** const tlsBuff = io_engine::getTlsValueBuff();
	if (tlsBuff)
	{
		ui_tls*& uiTls = (ui_tls*&)tlsBuff[QT_UI_TLS_INDEX];
		if (!uiTls)
		{//ios线程中创建的UI
			uiTls = new ui_tls();
		}
		uiTls->_count++;
	}
	else
	{
		ui_tls* uiTls = new ui_tls();
		uiTls->_count++;
		uiTls->_tlsBuff[QT_UI_TLS_INDEX] = uiTls;
		io_engine::setTlsBuff(uiTls->_tlsBuff);
	}
}

void bind_qt_run_base::ui_tls::reset()
{
	void** const tlsBuff = io_engine::getTlsValueBuff();
	assert(tlsBuff);
	ui_tls* uiTls = (ui_tls*)tlsBuff[QT_UI_TLS_INDEX];
	if (0 == --uiTls->_count)
	{
		if (tlsBuff == uiTls->_tlsBuff)
		{
			io_engine::setTlsBuff(NULL);
		}
		else
		{//ios线程中创建的UI
			tlsBuff[QT_UI_TLS_INDEX] = NULL;
		}
		delete uiTls;
	}
}

bind_qt_run_base::ui_tls* bind_qt_run_base::ui_tls::push_stack(bind_qt_run_base* s)
{
	assert(s);
	ui_tls* uiTls = (ui_tls*)io_engine::getTlsValue(QT_UI_TLS_INDEX);
	assert(uiTls);
	uiTls->_uiStack.push_front(s);
	return uiTls;
}

bind_qt_run_base* bind_qt_run_base::ui_tls::pop_stack()
{
	return pop_stack((ui_tls*)io_engine::getTlsValue(QT_UI_TLS_INDEX));
}

bind_qt_run_base* bind_qt_run_base::ui_tls::pop_stack(ui_tls* uiTls)
{
	assert(uiTls && uiTls == (ui_tls*)io_engine::getTlsValue(QT_UI_TLS_INDEX));
	assert(!uiTls->_uiStack.empty());
	bind_qt_run_base* r = uiTls->_uiStack.front();
	uiTls->_uiStack.pop_front();
	return r;
}

bool bind_qt_run_base::ui_tls::running_in_this_thread(bind_qt_run_base* s)
{
	void** const tlsBuff = io_engine::getTlsValueBuff();
	if (tlsBuff && tlsBuff[QT_UI_TLS_INDEX])
	{
		ui_tls* uiTls = (ui_tls*)tlsBuff[QT_UI_TLS_INDEX];
		for (bind_qt_run_base* const ele : uiTls->_uiStack)
		{
			if (ele == s)
			{
				return true;
			}
		}
	}
	return false;
}

size_t bind_qt_run_base::ui_tls::running_depth()
{
	size_t dp = 0;
	void** const tlsBuff = io_engine::getTlsValueBuff();
	if (tlsBuff && tlsBuff[QT_UI_TLS_INDEX])
	{
		ui_tls* uiTls = (ui_tls*)tlsBuff[QT_UI_TLS_INDEX];
		dp = uiTls->_uiStack.size();
	}
	return dp;
}
//////////////////////////////////////////////////////////////////////////

mem_alloc_mt2<bind_qt_run_base::task_event>* bind_qt_run_base::task_event::_taskAlloc = NULL;

void* bind_qt_run_base::task_event::operator new(size_t s)
{
	assert(sizeof(task_event) == s);
	return _taskAlloc->allocate();
}

void bind_qt_run_base::task_event::operator delete(void* p)
{
	_taskAlloc->deallocate(p);
}
//////////////////////////////////////////////////////////////////////////

void bind_qt_run_base::install()
{
	if (!task_event::_taskAlloc)
	{
		task_event::_taskAlloc = new mem_alloc_mt2<bind_qt_run_base::task_event>(256);
	}
}

void bind_qt_run_base::uninstall()
{
	delete task_event::_taskAlloc;
	task_event::_taskAlloc = NULL;
}

bind_qt_run_base::bind_qt_run_base()
:_eventLoop(NULL), _checkCloseHandler(NULL), _waitCount(0), _waitClose(false), _inCloseScope(false), _locked(false)
{
	DEBUG_OPERATION(_taskCount = 0);
	_threadID = run_thread::this_thread_id();
	ui_tls::init();
}

bind_qt_run_base::~bind_qt_run_base()
{
	assert(run_in_ui_thread());
	assert(!running_in_this_thread());
	assert(0 == _waitCount);
	assert(0 == _taskCount);
	assert(!_waitClose);
	assert(!_eventLoop);
	assert(!_checkCloseHandler);
	assert(!_locked);
	assert(_readyQueue.empty());
	assert(_waitQueue.empty());
	if (_qtStrand)
	{
		_qtStrand->release();
	}
	ui_tls::reset();
}

run_thread::thread_id bind_qt_run_base::thread_id()
{
	assert(run_thread::this_thread_id() != _threadID);
	return _threadID;
}

bool bind_qt_run_base::run_in_ui_thread()
{
	return run_thread::this_thread_id() == _threadID;
}

bool bind_qt_run_base::running_in_this_thread()
{
	return ui_tls::running_in_this_thread(this);
}

bool bind_qt_run_base::only_self()
{
	assert(running_in_this_thread());
	return 1 == ui_tls::running_depth();
}

bool bind_qt_run_base::is_wait_close()
{
	assert(run_in_ui_thread());
	return _waitClose;
}

bool bind_qt_run_base::wait_close_reached()
{
	assert(run_in_ui_thread());
	return 0 == _waitCount;
}

bool bind_qt_run_base::inside_wait_close_loop()
{
	assert(run_in_ui_thread());
	return NULL != _eventLoop;
}

bool bind_qt_run_base::in_close_scope()
{
	return _inCloseScope;
}

void bind_qt_run_base::set_in_close_scope_sign(bool b)
{
	_inCloseScope = b;
}

std::function<void()> bind_qt_run_base::wrap_check_close()
{
	assert(run_in_ui_thread());
	_waitCount++;
	return FUNCTION_ALLOCATOR(std::function<void()>, wrap([this]
	{
		check_close();
	}), (reusable_alloc<void, reusable_mem_mt<>>(_reuMem)));
}

void bind_qt_run_base::append_task(wrap_handler_face* h)
{
	_queueMutex.lock();
	if (_locked)
	{
		_waitQueue.push_back(h);
		_queueMutex.unlock();
	}
	else
	{
		_locked = true;
		_readyQueue.push_back(h);
		_queueMutex.unlock();
		post_task_event();
	}
}

void bind_qt_run_base::run_one_task()
{
	ui_tls* uiTls = ui_tls::push_stack(this);
	do
	{
		while (!_readyQueue.empty())
		{
			wrap_handler_face* h = static_cast<wrap_handler_face*>(_readyQueue.pop_front());
			h->invoke();
			_reuMem.deallocate(h);
		}
		_queueMutex.lock();
		if (!_waitQueue.empty())
		{
			_waitQueue.swap(_readyQueue);
			_queueMutex.unlock();
			if (is_wait_close())
			{
				continue;
			}
			post_task_event();
		}
		else
		{
			_locked = false;
			_queueMutex.unlock();
		}
		break;
	} while (true);
	bind_qt_run_base* r = ui_tls::pop_stack(uiTls);
	assert(this == r);
}

void bind_qt_run_base::enter_wait_close()
{
	assert(run_in_ui_thread());
	assert(!_waitClose);
	if (_waitCount)
	{
		_waitClose = true;
		enter_loop();
		_waitClose = false;
	}
}

void bind_qt_run_base::check_close()
{
	assert(_waitCount > 0);
	_waitCount--;
	if (_waitClose && 0 == _waitCount)
	{
		if (_checkCloseHandler)
		{
			wrap_handler_face* h = _checkCloseHandler;
			_checkCloseHandler = NULL;
			h->invoke();
			_reuMem.deallocate(h);
			_waitClose = false;
		}
		else
		{
			close_now();
		}
	}
}

void bind_qt_run_base::ui_yield(my_actor* host)
{
	host->trig_guard([this](trig_once_notifer<>&& cb)
	{
		append_task(make_wrap_handler(_reuMem, std::bind([](const trig_once_notifer<>& cb)
		{
			CHECK_EXCEPTION(cb);
		}, std::move(cb))));
	});
}

actor_handle bind_qt_run_base::create_ui_actor(const my_actor::main_func& mainFunc, size_t stackSize /*= QT_UI_ACTOR_STACK_SIZE*/)
{
	assert(!!_qtStrand);
	return my_actor::create(_qtStrand, mainFunc, stackSize);
}

actor_handle bind_qt_run_base::create_ui_actor(my_actor::main_func&& mainFunc, size_t stackSize /*= QT_UI_ACTOR_STACK_SIZE*/)
{
	assert(!!_qtStrand);
	return my_actor::create(_qtStrand, std::move(mainFunc), stackSize);
}

child_handle bind_qt_run_base::create_ui_child_actor(my_actor* host, const my_actor::main_func& mainFunc, size_t stackSize /*= QT_UI_ACTOR_STACK_SIZE*/)
{
	assert(!!_qtStrand);
	return host->create_child(_qtStrand, mainFunc, stackSize);
}

child_handle bind_qt_run_base::create_ui_child_actor(my_actor* host, my_actor::main_func&& mainFunc, size_t stackSize /*= QT_UI_ACTOR_STACK_SIZE*/)
{
	assert(!!_qtStrand);
	return host->create_child(_qtStrand, std::move(mainFunc), stackSize);
}

const shared_qt_strand& bind_qt_run_base::start_qt_strand(io_engine& ios)
{
	assert(run_in_ui_thread());
	if (!_qtStrand || &_qtStrand->get_io_engine() != &ios)
	{
		_qtStrand = qt_strand::create(ios, this);
	}
	return _qtStrand;
}

const shared_qt_strand& bind_qt_run_base::ui_strand()
{
	assert(!!_qtStrand);
	return _qtStrand;
}
#endif