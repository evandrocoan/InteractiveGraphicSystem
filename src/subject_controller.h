//
// All source code on this page Copyright (c) 2016 Miguel Ibero
// Licensed under the MIT License
// https://opensource.org/licenses/MIT
//
// Implementing Signals in C++11
// https://coderwall.com/p/u4w9ra/implementing-signals-in-c-11
//
/*

This C++11 implementation works exactly the same way as boost::signal and uses a std::shared_ptr to
share a SignalConnectionItem. This shared pointer is used by the connection and the signal to mark
the connection as disconnected but is not public. The signal template does not allow you to return
values other than void, which was a problem of boost::signal, and we also implemented a
ScopedConnection class for a simpler RAII idiom.

class GameScore
{
private:
  typedef Signal<unsigned> ChangedSignal;
  ChangedSignal _changedEvent;

public:
  typedef ChangedSignal::Callback ChangedCallback;
  typedef ChangedSignal::Connection ChangedConnection;
  typedef ChangedSignal::ScopedConnection ChangedScopedConnection;

  GameScore(unsigned value=0) : _value(value)
  {
  }

  void setValue(unsigned value)
  {
    _value = value;
    _changedEvent(value);
  }

  ChangedConnection addObserver(const ChangedCallback& callback)
  {
    return _changedEvent.connect(callback);
  }
};

This way the observer pattern implementation using the signal is not shown to the outside. We also
add each signal as a property to the object that is going to generate the event, removing the giant
signal list header. Now listening to this event is much cleaner.

class ScoreView
{
private:
  GameScore::ScopedConnection _scoreChangedConnection;

public:
  GameView(GameScore& score) :
  _scoreChangedConnection(score.addObserver(std::bind(&ScoreView::updateScore, this, std::placeholders::_1)))
  {
  }

  ~GameView()
  {
    // no need to disconnect when using ScopedConnection
  }

  void updateScore(unsigned score)
  {
    // update the score view
  }
};

*/


#ifndef OBSERVER_DESIGN_PATTER_SUBJECT_CONTROLLER_H
#define OBSERVER_DESIGN_PATTER_SUBJECT_CONTROLLER_H

#include <list>
#include <memory>

#include <functional>
#include <algorithm>

template<class... F>
class SignalConnection;

template<class... F>
class ScopedSignalConnection;

template<class... F>
class SignalConnectionItem
{
public:
  typedef std::function<void(F...)> Callback;
private:
  Callback _callback;
  bool _connected;

public:
  SignalConnectionItem(const Callback& cb, bool connected=true) :
  _callback(cb), _connected(connected)
  {
  }

  void operator()(F... args)
  {
    if(_connected && _callback)
    {
      _callback(args...);
    }
  }

  bool connected() const
  {
    return _connected;
  }

  void disconnect()
  {
    _connected = false;
  }
};

template<class... F>
class Signal
{
public:
  typedef std::function<void(F...)> Callback;
  typedef SignalConnection<F...> Connection;
  typedef ScopedSignalConnection<F...> ScopedConnection;

private:
  typedef SignalConnectionItem<F...> ConnectionItem;
  typedef std::list<std::shared_ptr<ConnectionItem>> ConnectionList;

  ConnectionList _list;
  unsigned _recurseCount;

  void clearDisconnected()
  {
    _list.erase(std::remove_if(_list.begin(), _list.end(), [](std::shared_ptr<ConnectionItem>& item){
      return !item->connected();
    }), _list.end());
  }

public:
  Signal() :
  _recurseCount(0)
  {
  }

  ~Signal()
  {
    for(auto& item : _list)
    {
      item->disconnect();
    }
  }

  void operator()(F... args)
  {
    std::list<std::shared_ptr<ConnectionItem>> list;

    for(auto& item : _list)
    {
      if(item->connected())
      {
        list.push_back(item);
      }
    }

    _recurseCount++;

    for(auto& item : list)
    {
      (*item)(args...);
    }

    _recurseCount--;

    if(_recurseCount == 0)
    {
      clearDisconnected();
    }
  };

  Connection connect(const Callback& callback)
  {
    auto item = std::make_shared<ConnectionItem>(callback, true);
    _list.push_back(item);

    return Connection(*this, item);
  }

  bool disconnect(const Connection& connection)
  {
    bool found = false;

    for(auto& item : _list)
    {
      if(connection.hasItem(*item) && item->connected())
      {
        found = true;
        item->disconnect();
      }
    }

    if(found)
    {
      clearDisconnected();
    }

    return found;
  }

  void disconnectAll()
  {
    for(auto& item : _list)
    {
      item->disconnect();
    }

    clearDisconnected();
  }

  friend class Connecion;
};

template<class... F>
class SignalConnection
{
private:
  typedef SignalConnectionItem<F...> Item;

  Signal<F...>* _signal;
  std::shared_ptr<Item> _item;

public:
  SignalConnection()
  : _signal(nullptr)
  {
  }

  SignalConnection(Signal<F...>& signal, const std::shared_ptr<Item>& item)
  : _signal(&signal), _item(item)
  {
  }

  void operator=(const SignalConnection& other)
  {
    _signal = other._signal;
    _item = other._item;
  }

  virtual ~SignalConnection()
  {
  }

  bool hasItem(const Item& item) const
  {
    return _item.get() == &item;
  }

  bool connected() const
  {
    return _item->connected;
  }

  bool disconnect()
  {
    if(_signal && _item && _item->connected())
    {
      return _signal->disconnect(*this);
    }
    return false;
  }
};

template<class... F>
class ScopedSignalConnection : public SignalConnection<F...>
{
public:

  ScopedSignalConnection()
  {
  }

  ScopedSignalConnection(Signal<F...>* signal, void* callback)
  : SignalConnection<F...>(signal, callback)
  {
  }

  ScopedSignalConnection(const SignalConnection<F...>& other)
  : SignalConnection<F...>(other)
  {
  }

  ~ScopedSignalConnection()
  {
    this->disconnect();
  }

  ScopedSignalConnection & operator=(const SignalConnection<F...>& connection)
  {
    this->disconnect();
    SignalConnection<F...>::operator=(connection);

    return *this;
  }
};

#endif // OBSERVER_DESIGN_PATTER_SUBJECT_CONTROLLER_H
