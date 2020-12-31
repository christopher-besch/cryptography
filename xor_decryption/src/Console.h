#pragma once

#include <vector>

// #include "Core.h"

// let's make hkarrson happy
class VectorArgument
{
private:
    // like '-b' or '--base'
    std::vector<const char *> m_init_arguments;
    // arguments following the init argument
    std::vector<const char *> m_arguments;
    // only search for this amount of arguments, -1 when infite amount
    int m_max_amount_arguments;
    // when false and not found -> raise error
    bool m_optional;
    // true when an init argument occurred somewhere
    bool m_found;

public:
    VectorArgument(std::vector<const char *> init_arguments, int max_amount_arguments, bool optional = true);

    void got_found() { m_found = true; }

    void add_argument(const char *argument);

    const std::vector<const char *> &get_init_arguments() const { return m_init_arguments; }
    const std::vector<const char *> &get_arguments() const { return m_arguments; }
    int get_max_amount_arguments() const { return m_max_amount_arguments; }
    bool is_required() const { return !m_optional; }
    const bool is_found() const { return m_found; }

    bool contains_init_argument(const char *init_argument) const;

    bool is_satisfied() const
    {
        return m_arguments.size() == m_max_amount_arguments;
    }
};

class ConsoleArguments
{
private:
    std::vector<VectorArgument> m_vector_arguments;
    // arguments without --something
    std::vector<const char *> m_other_arguments;

private:
    void all_required_found();

public:
    void add_optional(std::vector<const char *> init_arguments, int max_arguments = -1)
    {
        m_vector_arguments.push_back({init_arguments, max_arguments, true});
    }
    void add_required(std::vector<const char *> init_arguments, int max_arguments = -1)
    {
        m_vector_arguments.push_back({init_arguments, max_arguments, false});
    }
    // arguments that are either true or false, existend or non-existent
    void add_bool(std::vector<const char *> init_arguments)
    {
        m_vector_arguments.push_back({init_arguments, 0, true});
    }

    const VectorArgument &operator[](const char *init_argument) const;

    const char *operator[](int idx) const;

    void load_arguments(int argc, char *argv[]);
};
