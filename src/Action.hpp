#pragma once

#include <sstream>

class Action
{
    std::string m_name;
    std::string m_type;

  public:
    Action() = default;

    Action(const std::string &name, const std::string &type);

    const std::string &name() const;
    const std::string &type() const;
};