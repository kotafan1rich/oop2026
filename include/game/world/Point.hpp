#pragma once

class Point
{
  private:
    int x_;
    int y_;

  public:
    Point(int x, int y);
    ~Point();

    void setX(int x);
    void setY(int y);
    int getX() const;
    int getY() const;
    void setPoint(int x, int y);

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

