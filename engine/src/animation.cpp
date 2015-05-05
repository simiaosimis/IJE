/*
 * Implementação da classe Animation.
 *
 * Autor: Edson Alves
 * Data: 05/05/2015
 * Licença: LGPL. Sem copyright.
 */
#include "animation.h"

#include "rect.h"
#include "image.h"
#include "environment.h"

class Animation::Impl
{
public:
    Impl(const string& id, double x_, double y_, double w_, double h_, int f,
        unsigned long s, bool l_)
        : x(x_), y(y_), w(w_), h(h_), frames(f), speed(s), loop(l_),
        done(false), last(0), frame(0)
    {
        Environment *env = Environment::get_instance();
        image = env->resources_manager->get_image(id);
    }

    double x, y, w, h;
    int frames;
    unsigned long speed;
    bool loop, done;
    unsigned long last;
    int frame;
    shared_ptr<Image> image;
};

Animation::Animation(const string& image, double x, double y, double w,
    double h, int frames, unsigned long speed_in_ms, bool loop)
    : m_impl(new Animation::Impl(image, x, y, w, h, frames, speed_in_ms, loop))
{
}

Animation::~Animation()
{
}

bool
Animation::is_done() const
{
    return m_impl->done;
}

void
Animation::update(unsigned long elapsed)
{
    if (not m_impl->last)
    {
        m_impl->last = elapsed;
    }

    if (elapsed - m_impl->last > m_impl->speed)
    {
        ++m_impl->frame;
        m_impl->last += m_impl->speed;

        if (m_impl->loop)
        {
            m_impl->frame %= m_impl->frames;
        } else if (m_impl->frame == m_impl->frames)
        {
            --m_impl->frame;
            m_impl->done = true;
        }
    }
}

void
Animation::draw(double x, double y)
{
    if (m_impl->done)
    {
        return;
    }

    Rect clip { m_impl->x + m_impl->frame * m_impl->w, m_impl->y, m_impl->w,
        m_impl->h
              };

    Environment *env = Environment::get_instance();
    env->canvas->draw(m_impl->image.get(), clip, x, y);
}

double
Animation::w() const
{
    return m_impl->w;
}

double
Animation::h() const
{
    return m_impl->h;
}