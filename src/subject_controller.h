//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//
// https://github.com/juanchopanza/cppblog/blob/master/Patterns/Observer/Subject.h
// https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
//

#ifndef OBSERVER_DESIGN_PATTER_SUBJECT_H
#define OBSERVER_DESIGN_PATTER_SUBJECT_H

#include <functional>
#include <map>
#include <vector>
#include <utility> // for std::forward

template <typename Event>
class Subject
{
 public:
  Subject()=default;

  template <typename Observer>
  void registerObserver(const Event& event, Observer&& observer)
  {
    observers_[event].push_back(std::forward<Observer>(observer));
  }

  template <typename Observer>
  void registerObserver(Event&& event, Observer&& observer)
  {
    observers_[std::move(event)].push_back(std::forward<Observer>(observer));
  }

  void notify(const Event& event) const
  {
    for (const auto& obs : observers_.at(event)) obs();
  }

  // disallow copying and assigning
  Subject(const Subject&)=delete;
  Subject& operator=(const Subject&)=delete;

 private:
  std::map<Event, std::vector<std::function<void()>>> observers_;
};

#endif // OBSERVER_DESIGN_PATTER_SUBJECT_H
