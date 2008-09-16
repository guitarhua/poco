//
// Getter.h
//
// $Id: //poco/Main/WebWidgets/include/Poco/WebWidgets/Getter.h#2 $
//
// Library: WebWidgets
// Package: Core
// Module:  Getter
//
// Definition of the Getter class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef WebWidgets_Getter_INCLUDED
#define WebWidgets_Getter_INCLUDED


#include "Poco/WebWidgets/WebWidgets.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Any.h"
#include "Poco/MetaProgramming.h"


namespace Poco {
namespace WebWidgets {


class WebWidgets_API Getter: public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<Getter> Ptr;
public:	
	Getter()
	{
	}
	
	virtual Poco::Any get() const = 0;
};


template <class TObj, class TArg> 
class GetterImpl: public Getter
{
public:
	typedef TArg (TObj::*NotifyMethod)() const;

	GetterImpl(TObj* obj, NotifyMethod method):
		_object(obj), 
		_method(method)
	{
	}
	
	TArg operator()() const
	{
		return (_object->*_method)();
	}
	
	Poco::Any get() const
	{
		return this->operator();
	}

protected:
	TObj*        _object;
	NotifyMethod _method;
};


template <class TObj, class TArg, class TArgIn> 
class GetterImpl2: public Getter
{
public:
	typedef TArg (TObj::*NotifyMethod)(TArgIn) const;
	typedef typename TypeWrapper<TArgIn>::TYPE TArgInValue;
	
	GetterImpl2(TObj* obj, NotifyMethod method, TArgIn in):
		_object(obj), 
		_method(method),
		_value(in)
	{
	}
	
	TArg operator()() const
	{
		return (_object->*_method)(_value);
	}
	
	Poco::Any get() const
	{
		return this->operator();
	}

protected:
	TObj*        _object;
	NotifyMethod _method;
	TArgInValue  _value;
};

template <class TObj, class TArg, class TArgIn>
Getter* getter(const TObj* object, TArg (TObj::*method)(TArgIn in) const)
	/// "Constructor" function for a Getter.
{
	return new GetterImpl2(object, method, in);
}


template <class TObj, class TArg>
Getter* getter(const TObj* object, TArg (TObj::*method)() const)
	/// "Constructor" function for a Getter.
{
	return new GetterImpl(object, method);
}


} } // namespace Poco::WebWidgets


#endif // WebWidgets_Getter_INCLUDED
