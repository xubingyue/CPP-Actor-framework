#include "async_timer.h"
#include "scattered.h"
#include "io_engine.h"
#include "actor_timer.h"
#ifndef DISABLE_BOOST_TIMER
#ifdef DISABLE_HIGH_TIMER
#include <boost/asio/deadline_timer.hpp>
typedef boost::asio::deadline_timer timer_type;
typedef boost::posix_time::microseconds micseconds;
#else
#include <boost/chrono/system_clocks.hpp>
#include <boost/asio/high_resolution_timer.hpp>
typedef boost::asio::basic_waitable_timer<boost::chrono::high_resolution_clock> timer_type;
typedef boost::chrono::microseconds micseconds;
#endif
#else
#include "waitable_timer.h"
typedef WaitableTimerEvent_ timer_type;
typedef long long micseconds;
#endif

AsyncTimer_::AsyncTimer_(ActorTimer_* actorTimer)
:_actorTimer(actorTimer), _handler(NULL), _isInterval(false) {}

AsyncTimer_::~AsyncTimer_()
{
	assert(!_handler);
}

void AsyncTimer_::cancel()
{
	assert(self_strand()->running_in_this_thread());
	if (_handler)
	{
		_actorTimer->cancel(_timerHandle);
		_handler->destroy(_reuMem);
		_handler = NULL;
	}
}

bool AsyncTimer_::advance()
{
	assert(self_strand()->running_in_this_thread());
	if (_handler)
	{
		if (!_isInterval)
		{
			_actorTimer->cancel(_timerHandle);
			wrap_base* cb = _handler;
			_handler = NULL;
			cb->invoke(true);
			cb->destroy(_reuMem);
		}
		else if (!_handler->is_top_call())
		{
			_handler->invoke(true);
			return true;
		}
	}
	return false;
}

shared_strand AsyncTimer_::self_strand()
{
	return _actorTimer->_weakStrand.lock();
}

async_timer AsyncTimer_::clone()
{
	return self_strand()->make_timer();
}

void AsyncTimer_::timeout_handler()
{
	_timerHandle.reset();
	if (!_isInterval)
	{
		wrap_base* cb = _handler;
		_handler = NULL;
		cb->invoke();
		cb->destroy(_reuMem);
	}
	else
	{
		_handler->invoke();
	}
}
//////////////////////////////////////////////////////////////////////////

overlap_timer::overlap_timer(const shared_strand& strand)
:_weakStrand(strand->_weakThis), _looping(false), _timerCount(0),
_extMaxTick(0), _extFinishTime(-1), _handlerQueue(MEM_POOL_LENGTH)
{
#ifdef DISABLE_BOOST_TIMER
	_timer = new timer_type(strand->get_io_engine(), this);
#else
	_timer = new timer_type(strand->get_io_engine());
#endif
}

overlap_timer::~overlap_timer()
{
	assert(_handlerQueue.empty());
	delete (timer_type*)_timer;
}

void overlap_timer::_timeout(long long us, timer_handle& timerHandle, bool deadline)
{
	if (!_lockStrand)
	{
		_lockStrand = _weakStrand.lock();
#ifdef DISABLE_BOOST_TIMER
		_lockIos.create(_lockStrand->get_io_service());
#endif
	}
	assert(_lockStrand->running_in_this_thread());
	timerHandle._timestamp = get_tick_us();
	long long et = deadline ? us : (timerHandle._timestamp + us);
	if (et >= _extMaxTick)
	{
		_extMaxTick = et;
		timerHandle._queueNode = _handlerQueue.insert(_handlerQueue.end(), std::make_pair(et, &timerHandle));
	}
	else
	{
		timerHandle._queueNode = _handlerQueue.insert(std::make_pair(et, &timerHandle));
	}

	if (!_looping)
	{//定时器已经退出循环，重新启动定时器
		_looping = true;
		assert(_handlerQueue.size() == 1);
		_extFinishTime = et;
		timer_loop(et, et - timerHandle._timestamp);
	}
	else if ((unsigned long long)et < (unsigned long long)_extFinishTime)
	{//定时期限前于当前定时器期限，取消后重新计时
		boost::system::error_code ec;
		((timer_type*)_timer)->cancel(ec);
		_timerCount++;
		_extFinishTime = et;
		timer_loop(et, et - timerHandle._timestamp);
	}
}

void overlap_timer::_cancel(timer_handle& timerHandle)
{
	handler_queue::iterator itNode = timerHandle._queueNode;
	if (_handlerQueue.size() == 1)
	{
		_extMaxTick = 0;
		_handlerQueue.erase(itNode);
		//如果没有定时任务就退出定时循环
		boost::system::error_code ec;
		((timer_type*)_timer)->cancel(ec);
		_timerCount++;
		_looping = false;
	}
	else if (itNode->first == _extMaxTick)
	{
		_handlerQueue.erase(itNode++);
		if (_handlerQueue.end() == itNode)
		{
			itNode--;
		}
		_extMaxTick = itNode->first;
	}
	else
	{
		_handlerQueue.erase(itNode);
	}
}

void overlap_timer::cancel(timer_handle& timerHandle)
{
	assert(_weakStrand.lock()->running_in_this_thread());
	if (!timerHandle.is_null())
	{//删除当前定时器节点
		assert(_lockStrand);
		_cancel(timerHandle);
		timerHandle._handler->destroy(_reuMem);
		timerHandle.reset();
	}
}

bool overlap_timer::advance(timer_handle& timerHandle)
{
	assert(_weakStrand.lock()->running_in_this_thread());
	if (!timerHandle.is_null())
	{
		assert(_lockStrand);
		if (!timerHandle._isInterval)
		{
			_cancel(timerHandle);
			AsyncTimer_::wrap_base* cb = timerHandle._handler;
			timerHandle._handler = NULL;
			cb->invoke(true);
			cb->destroy(_reuMem);
			return true;
		}
		else if (!timerHandle._handler->is_top_call())
		{
			timerHandle._handler->invoke(true);
			return true;
		}
	}
	return false;
}

shared_strand overlap_timer::self_strand()
{
	return _weakStrand.lock();
}

void overlap_timer::timer_loop(long long abs, long long rel)
{
	int tc = ++_timerCount;
#ifdef DISABLE_BOOST_TIMER
	((timer_type*)_timer)->async_wait(micseconds(abs), micseconds(rel), tc);
#else
	boost::system::error_code ec;
	((timer_type*)_timer)->expires_from_now(micseconds(rel), ec);
#ifdef ENABLE_POST_FRONT
	((timer_type*)_timer)->async_wait(_lockStrand->wrap_asio_front([this, tc](const boost::system::error_code&)
#else
	((timer_type*)_timer)->async_wait(_lockStrand->wrap_asio([this, tc](const boost::system::error_code&)
#endif
	{
		event_handler(tc);
	}));
#endif
}

#ifdef DISABLE_BOOST_TIMER
void overlap_timer::post_event(int tc)
{
	assert(_lockStrand);
#ifdef ENABLE_POST_FRONT
	_lockStrand->post_front([this, tc]
#else
	_lockStrand->post([this, tc]
#endif
	{
		event_handler(tc);
		if (!_lockStrand)
		{
			_lockIos.destroy();
		}
	});
}
#endif

void overlap_timer::event_handler(int tc)
{
	assert(_lockStrand->running_in_this_thread());
	if (tc == _timerCount)
	{
		_extFinishTime = 0;
		while (!_handlerQueue.empty())
		{
			handler_queue::iterator iter = _handlerQueue.begin();
			long long ct = get_tick_us();
			if (iter->first > ct)
			{
				_extFinishTime = iter->first;
				timer_loop(_extFinishTime, _extFinishTime - ct);
				return;
			}
			else
			{
				timer_handle* const timerHandle = iter->second;
				_handlerQueue.erase(iter);
				if (!timerHandle->_isInterval)
				{
					AsyncTimer_::wrap_base* cb = timerHandle->_handler;
					timerHandle->_handler = NULL;
					cb->invoke();
					cb->destroy(_reuMem);
				}
				else
				{
					timerHandle->_handler->invoke();
				}
			}
		}
		_looping = false;
		_lockStrand.reset();
	}
	else if (tc == _timerCount - 1)
	{
		_lockStrand.reset();
	}
}