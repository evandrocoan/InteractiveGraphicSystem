#ifndef GTKMM_APP_NON_COMPIABLE_H
#define GTKMM_APP_NON_COMPIABLE_H

/**
 * Most concise way to disable copying class in C++11
 * https://stackoverflow.com/questions/23709690/most-concise-way-to-disable-copying-class-in-c11
 */
class NonCopyable
{
protected:
  NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(NonCopyable const &) = delete;
  void operator=(NonCopyable const &x) = delete;
};

#endif // GTKMM_APP_NON_COMPIABLE_H
