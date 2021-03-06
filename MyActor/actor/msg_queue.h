#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include "mem_pool.h"
#include <map>
#include <set>
#include <list>

template <typename T, typename TAlloc = mem_alloc<>>
class msg_queue
{
	struct node
	{
		__space_align char _data[sizeof(T)];
		node* _next;
	};

	typedef typename TAlloc::template rebind<node>::other allocator;
public:
	msg_queue(size_t poolSize = sizeof(void*))
		:_alloc(poolSize), _size(0), _head(NULL), _tail(NULL) {}

	~msg_queue()
	{
		clear();
	}

	template <typename... Args>
	void push_back(Args&&... args)
	{
		BEGIN_CHECK_EXCEPTION;
		void* newNode = _alloc.allocate();
		new(newNode)T(std::forward<Args>(args)...);
		new_back((node*)newNode);
		END_CHECK_EXCEPTION;
	}

	template <typename... Args>
	void push_front(Args&&... args)
	{
		BEGIN_CHECK_EXCEPTION;
		void* newNode = _alloc.allocate();
		new(newNode)T(std::forward<Args>(args)...);
		new_front((node*)newNode);
		END_CHECK_EXCEPTION;
	}

	T& front()
	{
		assert(_size && _head);
		return *as_ptype<T>(_head->_data);
	}

	T& back()
	{
		assert(_size && _tail);
		return *as_ptype<T>(_tail->_data);
	}

	void pop_front()
	{
		assert(_size);
		node* frontNode = _head;
		_head = _head->_next;
		if (0 == --_size)
		{
			assert(!_head);
			_tail = NULL;
		}
		as_ptype<T>(frontNode->_data)->~T();
		_alloc.deallocate(frontNode);
	}

	size_t size()
	{
		return _size;
	}

	bool empty()
	{
		return !_size;
	}

	void clear()
	{
		while (_head)
		{
			_size--;
			assert((int)_size >= 0);
			node* frontNode = _head;
			_head = _head->_next;
			as_ptype<T>(frontNode->_data)->~T();
			_alloc.deallocate(frontNode);
		}
		assert(0 == _size && !_head);
		_tail = NULL;
	}

	void expand_fixed(size_t fixedSize)
	{
		if (fixedSize > _alloc._poolMaxSize)
		{
			_alloc._poolMaxSize = fixedSize;
		}
	}

	size_t fixed_size()
	{
		return _alloc._poolMaxSize;
	}
private:
	void new_back(node* newNode)
	{
		newNode->_next = NULL;
		if (!_head)
		{
			_head = newNode;
		}
		else
		{
			_tail->_next = newNode;
		}
		_tail = newNode;
		_size++;
	}

	void new_front(node* newNode)
	{
		newNode->_next = _head;
		_head = newNode;
		if (!_tail)
		{
			_tail = newNode;
		}
		_size++;
	}
private:
	node* _head;
	node* _tail;
	size_t _size;
	allocator _alloc;
};

template <typename TAlloc>
class msg_queue<void, TAlloc>
{
public:
	msg_queue(size_t poolSize = sizeof(void*))
		:_poolSize(poolSize), _size(0) {}

	~msg_queue()
	{
		clear();
	}

	template <typename... Args>
	void push_back(Args&&... args)
	{
		_size++;
	}

	template <typename... Args>
	void push_front(Args&&... args)
	{
		_size++;
	}

	void_type front()
	{
		assert(_size);
		return void_type();
	}

	void_type back()
	{
		assert(_size);
		return void_type();
	}

	void pop_front()
	{
		assert(_size);
		_size--;
	}

	size_t size()
	{
		return _size;
	}

	bool empty()
	{
		return !_size;
	}

	void clear()
	{
		_size = 0;
	}

	void expand_fixed(size_t fixedSize)
	{
		if (fixedSize > _poolSize)
		{
			_poolSize = fixedSize;
		}
	}

	size_t fixed_size()
	{
		return _poolSize;
	}
private:
	size_t _size;
	size_t _poolSize;
};

template <typename TAlloc>
class msg_queue<void_type, TAlloc>: public msg_queue<void, TAlloc>
{
public:
	msg_queue(size_t poolSize = sizeof(void*))
		:msg_queue<void, TAlloc>(poolSize) {}
};

template <typename TAlloc>
class msg_queue<std::tuple<void_type>, TAlloc>: public msg_queue<void, TAlloc>
{
public:
	msg_queue(size_t poolSize = sizeof(void*))
		:msg_queue<void, TAlloc>(poolSize) {}
};

template <typename T>
class node_queue
{
public:
	struct node
	{
		friend node_queue;

		~node()
		{
			assert(!_has);
		}

		T& get()
		{
			assert(_has);
			return *(T*)_data;
		}

		template <typename... Args>
		void set(Args&&... args)
		{
			assert(!_has);
			BEGIN_CHECK_EXCEPTION;
			new(_data)T(std::forward<Args>(args)...);
			DEBUG_OPERATION(_has = true);
			END_CHECK_EXCEPTION;
		}

		void destroy()
		{
			assert(_has);
			((T*)_data)->~T();
			DEBUG_OPERATION(_has = false);
		}
	private:
		__space_align char _data[sizeof(T)];
		node* _next;
		DEBUG_OPERATION(bool _has = false);
	};
public:
	node_queue()
		:_size(0), _head(NULL), _tail(NULL) {}

	~node_queue()
	{
		assert(empty());
	}
public:
	void push_back(node* newNode)
	{
		newNode->_next = NULL;
		if (!_head)
		{
			_head = newNode;
		}
		else
		{
			_tail->_next = newNode;
		}
		_tail = newNode;
		_size++;
	}

	void push_front(node* newNode)
	{
		newNode->_next = _head;
		_head = newNode;
		if (!_tail)
		{
			_tail = newNode;
		}
		_size++;
	}

	T& front()
	{
		assert(_size && _head);
		return _head->get();
	}

	T& back()
	{
		assert(_size && _tail);
		return _tail->get();
	}

	node* pop_front()
	{
		assert(_size);
		node* frontNode = _head;
		_head = _head->_next;
		if (0 == --_size)
		{
			assert(!_head);
			_tail = NULL;
		}
		return frontNode;
	}

	size_t size()
	{
		return _size;
	}

	bool empty()
	{
		return !_size;
	}
private:
	node* _head;
	node* _tail;
	size_t _size;
};

class op_queue
{
public:
	struct face
	{
		friend op_queue;
	private:
		face* _next;
	};

	op_queue()
		:_head(NULL), _tail(NULL) {}

	~op_queue()
	{
		assert(empty());
	}
public:
	void push_back(face* newFace)
	{
		newFace->_next = NULL;
		if (!_head)
		{
			_head = newFace;
		}
		else
		{
			_tail->_next = newFace;
		}
		_tail = newFace;
	}

	void push_front(face* newFace)
	{
		newFace->_next = _head;
		_head = newFace;
		if (!_tail)
		{
			_tail = newFace;
		}
	}

	void push_back(op_queue& other)
	{
		if (this != &other)
		{
			if (!_head)
			{
				_head = other._head;
				_tail = other._tail;
				other._head = other._tail = NULL;
			}
			else if (other._head)
			{
				_tail->_next = other._head;
				_tail = other._tail;
				other._head = other._tail = NULL;
			}
		}
	}

	void push_front(op_queue& other)
	{
		if (this != &other)
		{
			if (!_head)
			{
				_head = other._head;
				_tail = other._tail;
				other._head = other._tail = NULL;
			}
			else if (other._head)
			{
				other._tail->_next = _head;
				_head = other._head;
				other._head = other._tail = NULL;
			}
		}
	}

	void swap(op_queue& other)
	{
		face* t1 = other._head;
		face* t2 = other._tail;
		other._head = _head;
		other._tail = _tail;
		_head = t1;
		_tail = t2;
	}

	face* front()
	{
		assert(_head);
		return _head;
	}

	face* back()
	{
		assert(_tail);
		return _tail;
	}

	face* pop_front()
	{
		assert(_head);
		face* frontFace = _head;
		_head = _head->_next;
		if (!_head)
		{
			_tail = NULL;
		}
		return frontFace;
	}

	bool empty()
	{
		return !_head;
	}
private:
	face* _head;
	face* _tail;
};
//////////////////////////////////////////////////////////////////////////

template <typename T>
class fixed_buffer
{
	struct node
	{
		void destroy()
		{
			as_ptype<T>(space)->~T();
#if (_DEBUG || DEBUG)
			memset(space, 0xcf, sizeof(space));
#endif
		}

		template <typename Arg>
		void set(Arg&& arg)
		{
			new(space)T(std::forward<Arg>(arg));
		}

		T& get()
		{
			return *as_ptype<T>(space);
		}

		__space_align char space[sizeof(T)];
	};
public:
	fixed_buffer(size_t maxSize)
	{
		assert(0 != maxSize);
		_size = 0;
		_index = 0;
		_maxSize = maxSize;
		_buffer = (node*)malloc(sizeof(node)*maxSize);
	}

	~fixed_buffer()
	{
		clear();
		free(_buffer);
	}
public:
	size_t size() const
	{
		return _size;
	}

	size_t max_size() const
	{
		return _maxSize;
	}

	bool empty() const
	{
		return 0 == _size;
	}

	bool full() const
	{
		return _maxSize == _size;
	}

	void clear()
	{
		for (size_t i = 0; i < _size; i++)
		{
			const size_t t = _index + i;
			_buffer[(t < _maxSize) ? t : (t - _maxSize)].destroy();
		}
		_index = 0;
		_size = 0;
	}

	T& front()
	{
		assert(!empty());
		const size_t i = _index + _size - 1;
		return _buffer[(i < _maxSize) ? i : (i - _maxSize)].get();
	}

	void pop_front()
	{
		assert(!empty());
		_size--;
		const size_t i = _index + _size;
		_buffer[(i < _maxSize) ? i : (i - _maxSize)].destroy();
	}

	template <typename Arg>
	void push_back(Arg&& arg)
	{
		assert(!full());
		const size_t i = (0 != _index) ? (_index - 1) : (_maxSize - 1);
		_buffer[i].set(std::forward<Arg>(arg));
		_index = i;
		_size++;
	}
private:
	size_t _maxSize;
	size_t _size;
	size_t _index;
	node* _buffer;
};

template <>
class fixed_buffer<void>
{
public:
	fixed_buffer(size_t maxSize)
	{
		assert(0 != maxSize);
		_size = 0;
		_maxSize = maxSize;
	}

	~fixed_buffer()
	{
		clear();
	}
public:
	size_t size() const
	{
		return _size;
	}

	size_t max_size() const
	{
		return _maxSize;
	}

	bool empty() const
	{
		return 0 == _size;
	}

	bool full() const
	{
		return _maxSize == _size;
	}

	void clear()
	{
		_size = 0;
	}

	void_type front()
	{
		assert(!empty());
		return void_type();
	}

	void pop_front()
	{
		assert(!empty());
		_size--;
	}

	template <typename... Args>
	void push_back(Args&&...)
	{
		assert(!full());
		_size++;
	}
private:
	size_t _maxSize;
	size_t _size;
};

template <>
class fixed_buffer<void_type>: public fixed_buffer<void>
{
public:
	fixed_buffer(size_t maxSize)
		:fixed_buffer<void>(maxSize){}
};

template <>
class fixed_buffer<std::tuple<void_type>>: public fixed_buffer<void>
{
public:
	fixed_buffer(size_t maxSize)
		:fixed_buffer<void>(maxSize){}
};
//////////////////////////////////////////////////////////////////////////

template <typename T, typename _All = pool_alloc<> >
class msg_list : public std::list<T, typename _All::template try_rebind<T>::other>
{
	typedef typename _All::template try_rebind<T>::other AlNod;
public:
	typedef AlNod alloc_type;
#ifdef _MSC_VER
	typedef std::_List_node<T, typename AlNod::pointer> node_type;
#elif __GNUG__
	typedef std::_List_node<T> node_type;
#endif
	typedef pool_alloc<node_type> node_alloc;
public:
	typedef std::list<T, AlNod> base_list;
public:
	explicit msg_list(size_t poolSize = sizeof(void*))
		:base_list(AlNod(poolSize)) {}

	explicit msg_list(AlNod& Al)
		:base_list(Al) {}

	msg_list(const msg_list& s)
		:base_list(s) {}

	msg_list(msg_list&& s)
		:base_list(std::move((base_list&)s)) {}

	template <typename Al>
	msg_list(const std::list<T, Al>& s, size_t poolSize = -1)
		: base_list(AlNod(-1 == poolSize ? s.size() : poolSize))
	{
		*this = s;
	}
public:
	msg_list& operator=(const msg_list& s)
	{
		(base_list&)*this = (const base_list&)s;
		return *this;
	}

	msg_list& operator=(msg_list&& s)
	{
		(base_list&)*this = std::move((base_list&)s);
		return *this;
	}

	template <typename Al>
	msg_list& operator=(const std::list<T, Al>& s)
	{
		base_list::clear();
		for (auto& i : s)
		{
			base_list::push_back(i);
		}
		return *this;
	}

	template <typename Al>
	void copy_to(std::list<T, Al>& d)
	{
		for (auto& i : (*this))
		{
			d.push_back(i);
		}
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename Tval, typename _All = pool_alloc<> >
class msg_map : public std::map<Tkey, Tval, std::less<Tkey>, typename _All::template try_rebind<std::pair<Tkey const, Tval> >::other>
{
	typedef std::less<Tkey> Tcmp;
	typedef typename _All::template try_rebind<std::pair<const Tkey, Tval> >::other AlNod;
public:
	typedef AlNod alloc_type;
#ifdef _MSC_VER
	typedef std::_Tree_node<std::pair<Tkey const, Tval>, typename AlNod::pointer> node_type;
#elif __GNUG__
	typedef std::_Rb_tree_node<std::pair<Tkey const, Tval> > node_type;
#endif
	typedef pool_alloc<node_type> node_alloc;
public:
	typedef std::map<Tkey, Tval, Tcmp, AlNod> base_map;
public:
	explicit msg_map(size_t poolSize = sizeof(void*))
		:base_map(Tcmp(), AlNod(poolSize)){}

	explicit msg_map(AlNod& al)
		:base_map(Tcmp(), al) {}

	msg_map(const msg_map& s)
		:base_map(s) {}

	msg_map(msg_map&& s)
		:base_map(std::move((base_map&)s)) {}

	template <typename Al>
	msg_map(const std::map<Tkey, Tval, Tcmp, Al>& s, size_t poolSize = -1)
		: base_map(Tcmp(), AlNod(-1 == poolSize ? s.size() : poolSize))
	{
		*this = s;
	}
public:
	msg_map& operator=(const msg_map& s)
	{
		(base_map&)*this = (const base_map&)s;
		return *this;
	}

	msg_map& operator=(msg_map&& s)
	{
		(base_map&)*this = std::move((base_map&)s);
		return *this;
	}

	template <typename Al>
	msg_map& operator=(const std::map<Tkey, Tval, Tcmp, Al>& s)
	{
		base_map::clear();
		for (auto& i : s)
		{
			base_map::insert(i);
		}
		return *this;
	}

	template <typename Al>
	void copy_to(std::map<Tkey, Tval, Tcmp, Al>& d)
	{
		for (auto& i : (*this))
		{
			d.insert(i);
		}
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename Tval, typename _All = pool_alloc<> >
class msg_multimap : public std::multimap<Tkey, Tval, std::less<Tkey>, typename _All::template try_rebind<std::pair<Tkey const, Tval> >::other>
{
	typedef std::less<Tkey> Tcmp;
	typedef typename _All::template try_rebind<std::pair<const Tkey, Tval> >::other AlNod;
public:
	typedef AlNod alloc_type;
#ifdef _MSC_VER
	typedef std::_Tree_node<std::pair<Tkey const, Tval>, typename AlNod::pointer> node_type;
#elif __GNUG__
	typedef std::_Rb_tree_node<std::pair<Tkey const, Tval> > node_type;
#endif
	typedef pool_alloc<node_type> node_alloc;
public:
	typedef std::multimap<Tkey, Tval, Tcmp, AlNod> base_map;
public:
	explicit msg_multimap(size_t poolSize = sizeof(void*))
		:base_map(Tcmp(), AlNod(poolSize)){}

	explicit msg_multimap(AlNod& al)
		:base_map(Tcmp(), al) {}

	msg_multimap(const msg_multimap& s)
		:base_map(s) {}

	msg_multimap(msg_multimap&& s)
		:base_map(std::move((base_map&)s)) {}

	template <typename Al>
	msg_multimap(const std::multimap<Tkey, Tval, Tcmp, Al>& s, size_t poolSize = -1)
		: base_map(Tcmp(), AlNod(-1 == poolSize ? s.size() : poolSize))
	{
		*this = s;
	}
public:
	msg_multimap& operator=(const msg_multimap& s)
	{
		(base_map&)*this = (const base_map&)s;
		return *this;
	}

	msg_multimap& operator=(msg_multimap&& s)
	{
		(base_map&)*this = std::move((base_map&)s);
		return *this;
	}

	template <typename Al>
	msg_multimap& operator=(const std::multimap<Tkey, Tval, Tcmp, Al>& s)
	{
		base_map::clear();
		for (auto& i : s)
		{
			base_map::insert(i);
		}
		return *this;
	}

	template <typename Al>
	void copy_to(std::multimap<Tkey, Tval, Tcmp, Al>& d)
	{
		for (auto& i : (*this))
		{
			d.insert(i);
		}
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename _All = pool_alloc<> >
class msg_set : public std::set<Tkey, std::less<Tkey>, typename _All::template try_rebind<Tkey>::other>
{
	typedef std::less<Tkey> Tcmp;
	typedef typename _All::template try_rebind<Tkey>::other AlNod;
public:
	typedef AlNod alloc_type;
#ifdef _MSC_VER
	typedef std::_Tree_node<Tkey, typename AlNod::pointer> node_type;
#elif __GNUG__
	typedef std::_Rb_tree_node<Tkey> node_type;
#endif
	typedef pool_alloc<node_type> node_alloc;
public:
	typedef std::set<Tkey, Tcmp, AlNod> base_set;
public:
	explicit msg_set(size_t poolSize = sizeof(void*))
		:base_set(Tcmp(), AlNod(poolSize)){}

	explicit msg_set(AlNod& al)
		:base_set(Tcmp(), al) {}

	msg_set(const msg_set& s)
		:base_set(s) {}

	msg_set(msg_set&& s)
		:base_set(std::move((base_set&)s)) {}

	template <typename Al>
	msg_set(const std::set<Tkey, Tcmp, Al>& s, size_t poolSize = -1)
		: base_set(Tcmp(), AlNod(-1 == poolSize ? s.size() : poolSize))
	{
		*this = s;
	}
public:
	msg_set& operator=(const msg_set& s)
	{
		(base_set&)*this = (const base_set&)s;
		return *this;
	}

	msg_set& operator=(msg_set&& s)
	{
		(base_set&)*this = std::move((base_set&)s);
		return *this;
	}

	template <typename Al>
	msg_set& operator=(const std::set<Tkey, Tcmp, Al>& s)
	{
		base_set::clear();
		for (auto& i : s)
		{
			base_set::insert(i);
		}
		return *this;
	}

	template <typename Al>
	void copy_to(std::set<Tkey, Tcmp, Al>& d)
	{
		for (auto& i : (*this))
		{
			d.insert(i);
		}
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename _All = pool_alloc<> >
class msg_multiset : public std::multiset<Tkey, std::less<Tkey>, typename _All::template try_rebind<Tkey>::other>
{
	typedef std::less<Tkey> Tcmp;
	typedef typename _All::template try_rebind<Tkey>::other AlNod;
public:
	typedef AlNod alloc_type;
#ifdef _MSC_VER
	typedef std::_Tree_node<Tkey, typename AlNod::pointer> node_type;
#elif __GNUG__
	typedef std::_Rb_tree_node<Tkey> node_type;
#endif
	typedef pool_alloc<node_type> node_alloc;
public:
	typedef std::multiset<Tkey, Tcmp, AlNod> base_set;
public:
	explicit msg_multiset(size_t poolSize = sizeof(void*))
		:base_set(Tcmp(), AlNod(poolSize)){}

	explicit msg_multiset(AlNod& al)
		:base_set(Tcmp(), al) {}

	msg_multiset(const msg_multiset& s)
		:base_set(s) {}

	msg_multiset(msg_multiset&& s)
		:base_set(std::move((base_set&)s)) {}

	template <typename Al>
	msg_multiset(const std::multiset<Tkey, Tcmp, Al>& s, size_t poolSize = -1)
		: base_set(Tcmp(), AlNod(-1 == poolSize ? s.size() : poolSize))
	{
		*this = s;
	}
public:
	msg_multiset& operator=(const msg_multiset& s)
	{
		(base_set&)*this = (const base_set&)s;
		return *this;
	}

	msg_multiset& operator=(msg_multiset&& s)
	{
		(base_set&)*this = std::move((base_set&)s);
		return *this;
	}

	template <typename Al>
	msg_multiset& operator=(const std::multiset<Tkey, Tcmp, Al>& s)
	{
		base_set::clear();
		for (auto& i : s)
		{
			base_set::insert(i);
		}
		return *this;
	}

	template <typename Al>
	void copy_to(std::multiset<Tkey, Tcmp, Al>& d)
	{
		for (auto& i : (*this))
		{
			d.insert(i);
		}
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename T, typename _All = pool_alloc_mt<> >
class msg_list_shared_alloc : public msg_list<T, typename _All::template rebind<typename msg_list<T>::node_type>::other>
{
	typedef typename _All::template rebind<typename msg_list<T>::node_type>::other AlNod;
	typedef msg_list<T, AlNod> base_list;
public:
	typedef AlNod shared_node_alloc;
public:
	explicit msg_list_shared_alloc(size_t poolSize = sizeof(void*))
		:base_list(poolSize) {}

	explicit msg_list_shared_alloc(AlNod& Al)
		: base_list(Al) {}

	msg_list_shared_alloc(const base_list& s)
		:base_list(s) {}

	msg_list_shared_alloc(msg_list_shared_alloc&& s)
		:base_list(std::move((base_list&)s)) {}

	template <typename Al>
	msg_list_shared_alloc(const std::list<T, Al>& s, size_t poolSize = -1)
		: base_list(s, poolSize) {}

	msg_list_shared_alloc& operator=(msg_list_shared_alloc&& s)
	{
		(base_list&)*this = std::move((base_list&)s);
		return *this;
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename Tval, typename _All = pool_alloc_mt<> >
class msg_map_shared_alloc : public msg_map<Tkey, Tval, typename _All::template rebind<typename msg_map<Tkey, Tval>::node_type>::other>
{
	typedef typename _All::template rebind<typename msg_map<Tkey, Tval>::node_type>::other AlNod;
	typedef msg_map<Tkey, Tval, AlNod> base_map;
public:
	typedef AlNod shared_node_alloc;
public:
	explicit msg_map_shared_alloc(size_t poolSize = sizeof(void*))
		:base_map(poolSize){}

	explicit msg_map_shared_alloc(AlNod& al)
		:base_map(al) {}

	msg_map_shared_alloc(const base_map& s)
		:base_map(s) {}

	msg_map_shared_alloc(msg_map_shared_alloc&& s)
		:base_map(std::move((base_map&)s)) {}

	template <typename Al, typename Tcmp>
	msg_map_shared_alloc(const std::map<Tkey, Tval, Tcmp, Al>& s, size_t poolSize = -1)
		: base_map(s, poolSize) {}

	msg_map_shared_alloc& operator=(msg_map_shared_alloc&& s)
	{
		(base_map&)*this = std::move((base_map&)s);
		return *this;
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename Tval, typename _All = pool_alloc_mt<> >
class msg_multimap_shared_alloc : public msg_multimap<Tkey, Tval, typename _All::template rebind<typename msg_multimap<Tkey, Tval>::node_type>::other>
{
	typedef typename _All::template rebind<typename msg_multimap<Tkey, Tval>::node_type>::other AlNod;
	typedef msg_multimap<Tkey, Tval, AlNod> base_map;
public:
	typedef AlNod shared_node_alloc;
public:
	explicit msg_multimap_shared_alloc(size_t poolSize = sizeof(void*))
		:base_map(poolSize){}

	explicit msg_multimap_shared_alloc(AlNod& al)
		:base_map(al) {}

	msg_multimap_shared_alloc(const base_map& s)
		:base_map(s) {}

	msg_multimap_shared_alloc(msg_multimap_shared_alloc&& s)
		:base_map(std::move((base_map&)s)) {}

	template <typename Al, typename Tcmp>
	msg_multimap_shared_alloc(const std::map<Tkey, Tval, Tcmp, Al>& s, size_t poolSize = -1)
		: base_map(s, poolSize) {}

	msg_multimap_shared_alloc& operator=(msg_multimap_shared_alloc&& s)
	{
		(base_map&)*this = std::move((base_map&)s);
		return *this;
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename _All = pool_alloc_mt<> >
class msg_set_shared_alloc : public msg_set<Tkey, typename _All::template rebind<typename msg_set<Tkey>::node_type>::other>
{
	typedef typename _All::template rebind<typename msg_set<Tkey>::node_type>::other AlNod;
	typedef msg_set<Tkey, AlNod> base_set;
public:
	typedef AlNod shared_node_alloc;
public:
	explicit msg_set_shared_alloc(size_t poolSize = sizeof(void*))
		:base_set(poolSize){}

	explicit msg_set_shared_alloc(AlNod& al)
		:base_set(al) {}

	msg_set_shared_alloc(const base_set& s)
		:base_set(s) {}

	msg_set_shared_alloc(msg_set_shared_alloc&& s)
		:base_set(std::move((base_set&)s)) {}

	template <typename Al, typename Tcmp>
	msg_set_shared_alloc(const std::set<Tkey, Tcmp, Al>& s, size_t poolSize = -1)
		: base_set(s, poolSize) {}

	msg_set_shared_alloc& operator=(msg_set_shared_alloc&& s)
	{
		(base_set&)*this = std::move((base_set&)s);
		return *this;
	}
};
//////////////////////////////////////////////////////////////////////////

template <typename Tkey, typename _All = pool_alloc_mt<> >
class msg_multiset_shared_alloc : public msg_multiset<Tkey, typename _All::template rebind<typename msg_multiset<Tkey>::node_type>::other>
{
	typedef typename _All::template rebind<typename msg_multiset<Tkey>::node_type>::other AlNod;
	typedef msg_multiset<Tkey, AlNod> base_set;
public:
	typedef AlNod shared_node_alloc;
public:
	explicit msg_multiset_shared_alloc(size_t poolSize = sizeof(void*))
		:base_set(poolSize){}

	explicit msg_multiset_shared_alloc(AlNod& al)
		:base_set(al) {}

	msg_multiset_shared_alloc(const base_set& s)
		:base_set(s) {}

	msg_multiset_shared_alloc(msg_multiset_shared_alloc&& s)
		:base_set(std::move((base_set&)s)) {}

	template <typename Al, typename Tcmp>
	msg_multiset_shared_alloc(const std::set<Tkey, Tcmp, Al>& s, size_t poolSize = -1)
		: base_set(s, poolSize) {}

	msg_multiset_shared_alloc& operator=(msg_multiset_shared_alloc&& s)
	{
		(base_set&)*this = std::move((base_set&)s);
		return *this;
	}
};

#endif