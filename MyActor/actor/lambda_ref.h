#ifndef __LAMBDA_REF_H
#define __LAMBDA_REF_H

//用户内嵌lambda的外部变量引用捕获，可以减小直接"&捕获"sizeof(lambda)的大小，提高调度效率
#define LAMBDA_REF1(__NAME__, __P0__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
} __NAME__ = { __P0__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF2(__NAME__, __P0__, __P1__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
} __NAME__ = { __P0__, __P1__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF3(__NAME__, __P0__, __P1__, __P2__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
} __NAME__ = { __P0__, __P1__, __P2__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF4(__NAME__, __P0__, __P1__, __P2__, __P3__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF5(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF6(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__, __P5__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF7(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF8(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF9(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
	typedef decltype(__P8__) _BOND_LR_(type8, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
	_BOND_LR_(type8, __LINE__)& __P8__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_REF10(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__, __P9__)\
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
	typedef decltype(__P8__) _BOND_LR_(type8, __LINE__); \
	typedef decltype(__P9__) _BOND_LR_(type9, __LINE__); \
struct\
{\
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
	_BOND_LR_(type8, __LINE__)& __P8__; \
	_BOND_LR_(type9, __LINE__)& __P9__; \
} __NAME__ = { __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__, __P9__ };
//////////////////////////////////////////////////////////////////////////

//用户内嵌lambda的外部变量引用捕获（外带当前this捕获，通过->访问），可以减小直接"&捕获"sizeof(lambda)的大小，提高调度效率
#define LAMBDA_THIS_REF1(__NAME__, __P0__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
} __NAME__ = { this, __P0__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF2(__NAME__, __P0__, __P1__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
} __NAME__ = { this, __P0__, __P1__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF3(__NAME__, __P0__, __P1__, __P2__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
} __NAME__ = { this, __P0__, __P1__, __P2__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF4(__NAME__, __P0__, __P1__, __P2__, __P3__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF5(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF6(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF7(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF8(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF9(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
	typedef decltype(__P8__) _BOND_LR_(type8, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
	_BOND_LR_(type8, __LINE__)& __P8__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__ };
//------------------------------------------------------------------------------------------

#define LAMBDA_THIS_REF10(__NAME__, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__, __P9__)\
	typedef decltype(this) this_type; \
	typedef decltype(__P0__) _BOND_LR_(type0, __LINE__); \
	typedef decltype(__P1__) _BOND_LR_(type1, __LINE__); \
	typedef decltype(__P2__) _BOND_LR_(type2, __LINE__); \
	typedef decltype(__P3__) _BOND_LR_(type3, __LINE__); \
	typedef decltype(__P4__) _BOND_LR_(type4, __LINE__); \
	typedef decltype(__P5__) _BOND_LR_(type5, __LINE__); \
	typedef decltype(__P6__) _BOND_LR_(type6, __LINE__); \
	typedef decltype(__P7__) _BOND_LR_(type7, __LINE__); \
	typedef decltype(__P8__) _BOND_LR_(type8, __LINE__); \
	typedef decltype(__P9__) _BOND_LR_(type9, __LINE__); \
struct\
{\
	this_type get(){ return __this; }\
	this_type operator->(){ return __this; }\
	this_type __this; \
	_BOND_LR_(type0, __LINE__)& __P0__; \
	_BOND_LR_(type1, __LINE__)& __P1__; \
	_BOND_LR_(type2, __LINE__)& __P2__; \
	_BOND_LR_(type3, __LINE__)& __P3__; \
	_BOND_LR_(type4, __LINE__)& __P4__; \
	_BOND_LR_(type5, __LINE__)& __P5__; \
	_BOND_LR_(type6, __LINE__)& __P6__; \
	_BOND_LR_(type7, __LINE__)& __P7__; \
	_BOND_LR_(type8, __LINE__)& __P8__; \
	_BOND_LR_(type9, __LINE__)& __P9__; \
} __NAME__ = { this, __P0__, __P1__, __P2__, __P3__, __P4__, __P5__, __P6__, __P7__, __P8__, __P9__ };

#define _PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N
#define _PP_NARG(...) _BOND_LR(_PP_ARG_N(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0),)

#define _BOND_LR(a, b) a##b
#define _BOND_LR_(a, b) _BOND_LR(a, b)
#define _BOND_LR__(a, b) _BOND_LR_(a, b)
//用户内嵌lambda的外部变量引用捕获，可以减小直接"&捕获"sizeof(lambda)的大小，提高调度效率
#define LAMBDA_REF(__NAME__, ...) _BOND_LR__(LAMBDA_REF, _PP_NARG(__VA_ARGS__))(__NAME__, __VA_ARGS__)
//用户内嵌lambda的外部变量引用捕获（外带当前this捕获，通过->访问），可以减小直接"&捕获"sizeof(lambda)的大小，提高调度效率
#define LAMBDA_THIS_REF(__NAME__, ...) _BOND_LR__(LAMBDA_THIS_REF, _PP_NARG(__VA_ARGS__))(__NAME__, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

template <typename... _Types>
struct LocalRecursive_;
template <typename... _Types>
struct LocalRecursiveFace_;
template <typename Handler, typename _Rt, typename... _Types>
struct LocalRecursiveInvoker_;

template <typename _Rt, typename... _Types>
struct LocalRecursiveFace_<_Rt(_Types...)>
{
	virtual _Rt invoke(_Types...) = 0;
	virtual void destroy() = 0;
};

template <typename Handler, typename _Rt, typename... _Types>
struct LocalRecursiveInvoker_<Handler, _Rt(_Types...)> : public LocalRecursiveFace_<_Rt(_Types...)>
{
	LocalRecursiveInvoker_(Handler& handler)
		:_handler(handler) {}

	_Rt invoke(_Types... args)
	{
		return _handler(std::forward<_Types>(args)...);
	}

	void destroy()
	{
		this->~LocalRecursiveInvoker_();
		DEBUG_OPERATION(memset(this, 0xcf, sizeof(*this)));
	}

	Handler& _handler;
};

template <typename _Rt, typename... _Types>
struct LocalRecursive_<_Rt(_Types...)>
{
	typedef LocalRecursiveFace_<_Rt(_Types...)> func_type;
	LocalRecursive_() {}
	~LocalRecursive_() { assert(!_depth && !_func); }

	template <typename... Args>
	_Rt operator()(Args&&... args)
	{
		DEBUG_OPERATION(_depth++);
		DEBUG_OPERATION(BREAK_OF_SCOPE({ _depth--; }));
		assert(_func);
		return _func->invoke(std::forward<Args>(args)...);
	}

	template <typename Handler>
	void set_handler(void* space, Handler& handler)
	{
		typedef LocalRecursiveInvoker_<Handler, _Rt(_Types...)> invoker_type;
		static_assert(sizeof(LocalRecursiveInvoker_<null_handler<>, void()>) == sizeof(invoker_type), "");
		assert(!_func);
		_func = new(space)invoker_type(handler);
	}

	void destroy()
	{
		assert(_func);
		_func->destroy();
		_func = NULL;
	}

	DEBUG_OPERATION(size_t _depth = 0);
	func_type* _func = NULL;
	NONE_COPY(LocalRecursive_);
};

#define DEFINE_LOCAL_RECURSIVE(__name__, __type__)\
	LocalRecursive_<__type__> __name__;

#define BEGIN_SET_RECURSIVE_FUNC(__name__)\
	const auto BOND_NAME(__lambda_, __name__) =

#define END_SET_RECURSIVE_FUNC(__name__); \
	__space_align char BOND_NAME(__space_, __name__)[sizeof(LocalRecursiveInvoker_<null_handler<>, void()>)]; \
	__name__.set_handler(BOND_NAME(__space_, __name__), BOND_NAME(__lambda_, __name__)); \
	BREAK_OF_SCOPE_NAME(BOND_NAME(__destroy_, __name__), { __name__.destroy(); });

#define SET_RECURSIVE_FUNC(__name__, __lmd__)\
	BEGIN_SET_RECURSIVE_FUNC(__name__) __lmd__; \
	END_SET_RECURSIVE_FUNC(__name__)

#define LOCAL_RECURSIVE(__name__, __type__, __lmd__)\
	DEFINE_LOCAL_RECURSIVE(__name__, __type__); \
	SET_RECURSIVE_FUNC(__name__, __lmd__) \

#define LOCAL_ACTOR(__name__, __lmd__)\
	LOCAL_RECURSIVE(__name__, void(my_actor*), __lmd__)

#define BEGIN_RECURSIVE(__name__, __type__)\
	DEFINE_LOCAL_RECURSIVE(__name__, __type__); \
	BEGIN_SET_RECURSIVE_FUNC(__name__)

#define END_RECURSIVE(__name__) END_SET_RECURSIVE_FUNC(__name__)

#define BEGIN_RECURSIVE_ACTOR(__name__, __self__) BEGIN_RECURSIVE(__name__, void(my_actor*)) [&](my_actor* __self__) {

#define END_RECURSIVE_ACTOR(__name__)  } END_RECURSIVE(__name__)

#define WRAP_LAMBDA_REF(__name__, __lmd__)\
	auto BOND_NAME(__lambda, __name__) = __lmd__; \
	auto __name__ = wrap_ref_handler(BOND_NAME(__lambda, __name__));

template <typename Handler, typename R>
struct OnceHandler_
{
	template <typename H>
	OnceHandler_(bool, H&& h)
		:_handler(TRY_MOVE(h)) {}

	OnceHandler_(const OnceHandler_<Handler, R>& s)
		:_handler(std::move(s._handler)) {}

	template <typename... Args>
	R operator()(Args&&... args)
	{
		return _handler(TRY_MOVE(args)...);
	}

	template <typename... Args>
	R operator()(Args&&... args) const
	{
		return _handler(TRY_MOVE(args)...);
	}

	mutable Handler _handler;
};

template <typename Handler>
struct OnceHandler_<Handler, void>
{
	template <typename H>
	OnceHandler_(bool, H&& h)
		:_handler(TRY_MOVE(h)) {}

	OnceHandler_(const OnceHandler_<Handler, void>& s)
		:_handler(std::move(s._handler)) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		_handler(TRY_MOVE(args)...);
	}

	template <typename... Args>
	void operator()(Args&&... args) const
	{
		_handler(TRY_MOVE(args)...);
	}

	mutable Handler _handler;
};

template <typename R = void, typename Handler>
OnceHandler_<RM_REF(Handler), R> wrap_once_handler(Handler&& handler)
{
	return OnceHandler_<RM_REF(Handler), R>(bool(), TRY_MOVE(handler));
}

template <typename Handler, typename R>
struct RefHandler_
{
	RefHandler_(bool, Handler& h)
	:_handler(h) {}

	template <typename... Args>
	R operator()(Args&&... args)
	{
		return _handler(TRY_MOVE(args)...);
	}

	template <typename... Args>
	R operator()(Args&&... args) const
	{
		return _handler(TRY_MOVE(args)...);
	}

	Handler& _handler;
};

template <typename Handler>
struct RefHandler_<Handler, void>
{
	RefHandler_(bool, Handler& h)
		:_handler(h) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		_handler(TRY_MOVE(args)...);
	}

	template <typename... Args>
	void operator()(Args&&... args) const
	{
		_handler(TRY_MOVE(args)...);
	}

	Handler& _handler;
};

template <typename R = void, typename Handler>
RefHandler_<Handler, R> wrap_ref_handler(Handler& handler)
{
	static_assert(!std::is_rvalue_reference<Handler&>::value, "");
	return RefHandler_<Handler, R>(bool(), handler);
}

template <typename... _Types>
struct wrap_local_handler_face;

template <typename Handler, typename _Rt, typename... _Types>
struct WrapLocalHandler_;

template <typename _Rt, typename... _Types>
struct wrap_local_handler_face<_Rt(_Types...)>
{
	virtual _Rt operator()(_Types...) = 0;
	virtual _Rt operator()(_Types...) const = 0;
};

template <typename Handler, typename _Rt, typename... _Types>
struct WrapLocalHandler_<Handler, _Rt(_Types...)> : public wrap_local_handler_face<_Rt(_Types...)>
{
	WrapLocalHandler_(Handler& handler)
	:_handler(handler) {}

	_Rt operator()(_Types... args)
	{
		return _handler(std::forward<_Types>(args)...);
	}

	_Rt operator()(_Types... args) const
	{
		return _handler(std::forward<_Types>(args)...);
	}

	Handler& _handler;
	NONE_COPY(WrapLocalHandler_);
	RVALUE_CONSTRUCT(WrapLocalHandler_, _handler);
};

template <typename Handler, typename... _Types>
struct WrapLocalHandler_<Handler, void(_Types...)> : public wrap_local_handler_face<void(_Types...)>
{
	WrapLocalHandler_(Handler& handler)
	:_handler(handler) {}

	void operator()(_Types... args)
	{
		_handler(std::forward<_Types>(args)...);
	}

	void operator()(_Types... args) const
	{
		_handler(std::forward<_Types>(args)...);
	}

	Handler& _handler;
	NONE_COPY(WrapLocalHandler_);
	RVALUE_CONSTRUCT(WrapLocalHandler_, _handler);
};

template <typename R = void, typename... Types, typename Handler>
WrapLocalHandler_<RM_REF(Handler), R(Types...)> wrap_local_handler(Handler&& handler)
{
	return WrapLocalHandler_<RM_REF(Handler), R(Types...)>(handler);
}

#endif