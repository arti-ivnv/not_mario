#include "Physics.hpp"
#include <iostream>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the overlap rectangle size of the bounding boxes of entity a and
    auto &a_transform = a->getComponent<CTransform>();
    auto &b_transform = b->getComponent<CTransform>();
    auto &a_bb        = a->getComponent<CBoungingBox>();
    auto &b_bb        = b->getComponent<CBoungingBox>();

    Vec2 delta(abs(a_transform.pos.x - b_transform.pos.x), abs(a_transform.pos.y - b_transform.pos.y));

    auto x_overlap = (a_bb.halfSize.x + b_bb.halfSize.x - delta.x);
    // std::cout << "x_overlap: " << x_overlap << std::endl;
    auto y_overlap = (a_bb.halfSize.y + b_bb.halfSize.y - delta.y);
    // std::cout << "y_overlap: " << y_overlap << std::endl;
    return Vec2(x_overlap, y_overlap);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
    //       previouys overlap uses the entity's previous position
    auto &a_transform = a->getComponent<CTransform>();
    auto &b_transform = b->getComponent<CTransform>();
    auto &a_bb        = a->getComponent<CBoungingBox>();
    auto &b_bb        = b->getComponent<CBoungingBox>();
    Vec2  delta(abs(a_transform.prevPos.x - b_transform.prevPos.x), abs(a_transform.prevPos.y - b_transform.prevPos.y));

    auto x_overlap = (a_bb.halfSize.x + b_bb.halfSize.x - delta.x);
    // std::cout << "x_prev_overlap: " << x_overlap << std::endl;
    auto y_overlap = (a_bb.halfSize.y + b_bb.halfSize.y - delta.y);
    // std::cout << "y_prev_overlap: " << y_overlap << std::endl;
    return Vec2(x_overlap, y_overlap);
};