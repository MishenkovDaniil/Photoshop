#include "plot.h"
#include <cmath>

Plot::Plot (Point &start, int x_len, int y_len, double max_val, Color color) 
          : size_ (x_len),
            start_ (start), 
            color_ (color),
            ordinate_len_ (y_len),
            abscissa_len_ (x_len),
            max_val_ (max_val)
{
    if (size_ <= 0)
    {
        fprintf (stderr, "Error: size (aka x_len) = %d is wrong for making plot", x_len);
        return;
    }
    coord_system = Coord_system (start_.x_, start_.y_, abscissa_len_, ordinate_len_, 0, 0, sf::VideoMode (abscissa_len_, ordinate_len_));
    
    array = new sf::VertexArray (sf::Points, size_);

    points_ = new Point[size_];
    assert (points_);
}

Plot::~Plot ()
{
    size_ = INT64_MIN;
    delete[] points_;
    delete array;
    points_ = nullptr;
}

void Plot::draw (sf::RenderTarget &target, double param)
{
    // static sf::VertexArray array (sf::Points, size_);
    double coeff = max_val_ / ordinate_len_;
    if (cur == size_)
        is_filled = true;
    if (!is_filled)
    {
        (*array)[cur].position = sf::Vector2f (start_.x_  + cur, start_.y_ - param / coeff);
        (*array)[cur].color = sf::Color::Red;
        points_[cur] = Point (start_.x_ + cur, start_.y_ - param / coeff);
        cur++;
    }
    else
    {
        // for (int point_num = (cur + 1) % size_; point_num < size_; ++point_num)
        // {
        //     int idx = point_num - (cur + 1) % size_;

        //     array[idx].position.x = start_.x_ + idx;
        //     array[idx].color = sf::Color::Red;
        //     array[idx].position.y = points_[point_num].y_;
        // }
        // for (int point_num = 0; point_num < cur % size_; ++point_num)
        // {
        //     int idx = size_ - (cur % size_) + point_num;
        //     array[idx].position.x = start_.x_ + idx;
        //     array[idx].color = sf::Color::Red;
        //     array[idx].position.y = points_[point_num].y_;
        // }

        // array[size_ - 1].position.y = start_.y_ - param; //+ cur val
        // points_[cur % size_] = Point (start_.x_ + size_ - 1, start_.y_ - param);

        for (int i = 0; i < size_ - 1; ++i)
        {
            (*array)[i].position.y = points_[i + 1].y_;
            points_[i] = points_[i + 1];
            (*array)[i].color = sf::Color::Red;
        }
        points_[size_ - 1] = Point (start_.x_ + size_ - 1, start_.y_ - param / coeff);
        (*array)[size_ - 1].position.y = start_.y_ - param / coeff;
    }

    Math_vector ordinate (0, ordinate_len_, sf::Color::White);
    Math_vector abscissa (abscissa_len_, 0, sf::Color::White);

    ordinate.draw (coord_system, target);
    abscissa.draw (coord_system, target);
    target.draw (*array);
    // coord_system.draw (target);
}