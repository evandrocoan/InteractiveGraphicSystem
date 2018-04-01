
#include <iostream>
#include "src/subject_controller.h"

struct Transformation
{
  Transformation()
  {
    std::cout << "3. Hi, I am a transformation." << std::endl;
  }
};

struct ViewWindow
{
  typedef Signal<Transformation> ChangedSignal;
  typedef ChangedSignal::Callback ChangedCallback;
  typedef ChangedSignal::Connection ChangedConnection;

  ChangedSignal _observers;

  ViewWindow()
  {
    std::cout << "1. Hi, I am the ViewWindow and I have a bunch of observers." << std::endl;
  }

  ChangedConnection addObserver(ChangedCallback callback)
  {
    ChangedConnection connection = this->_observers.connect(callback);
    this->callObservers();
    return connection;
  }

  void callObservers()
  {
    Transformation transformation;
    this->_observers(transformation);
  }
};

struct DrawingArea
{
  ViewWindow viewWindow;
  ViewWindow::ChangedConnection connection;

  DrawingArea() : viewWindow()
  {
    std::cout << "2. Hi, I am a drawing area." << std::endl;
    this->connection = this->viewWindow.addObserver(std::bind(&DrawingArea::updateObject, this, std::placeholders::_1));
  }

  ~DrawingArea()
  {
    bool disconnection_result = this->connection.disconnect();
    std::cout << "5. Hi, I am disconnecting for today's job: " << disconnection_result << std::endl;
  }

  void updateObject(Transformation transformation)
  {
    std::cout << "4. Hi, I do drawing updates." << std::endl;
  }
};

int main(int argc, char *argv[])
{
  DrawingArea drawingArea;
}
