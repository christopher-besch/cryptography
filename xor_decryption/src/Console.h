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
    VectorArgument(std::vector<const char *> init_arguments, int max_amount_arguments, bool optional = true)
        : m_init_arguments(init_arguments), m_max_amount_arguments(max_amount_arguments), m_optional(optional), m_found(false)
    {
        // todo: requested amount of arguments
        // checks
        if (m_init_arguments.empty())
            raise_error("Trying to create VectorArgument with no init arguments!");
        for (const char *init_argument : init_arguments)
            if (init_argument[0] != '-')
                raise_error("Init arguments have to start with '-'! '" << init_argument << "' is invalid!")
    }

    void got_found() { m_found = true; }

    void add_argument(const char *argument)
    {
        if (m_arguments.size() == m_max_amount_arguments)
            raise_error("Trying to add more arguments than allowed!");
        m_arguments.push_back(argument);
    }

    const std::vector<const char *> &get_init_arguments() const { return m_init_arguments; }
    const std::vector<const char *> &get_arguments() const { return m_arguments; }
    int get_max_amount_arguments() const { return m_max_amount_arguments; }
    bool is_required() const { return !m_optional; }
    const bool is_found() const { return m_found; }

    bool contains_init_argument(const char *init_argument) const
    {
        // look mum I managed to cram three good lines of code into one bad one
        return std::find_if(m_init_arguments.begin(), m_init_arguments.end(), [&](const char *this_init_argument) { return std::strcmp(this_init_argument, init_argument) == 0; }) != m_init_arguments.end();
    }

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
    void all_required_found()
    {
        for (VectorArgument &vector_argument : m_vector_arguments)
            if (vector_argument.is_required() && !vector_argument.is_found())
                raise_error("Console Parameter " << vector_argument.get_init_arguments()[0] << " is missing!");
    }

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

    const VectorArgument &operator[](const char *init_argument) const
    {
        for (const VectorArgument &vector_argument : m_vector_arguments)
            if (vector_argument.contains_init_argument(init_argument))
                return vector_argument;
        raise_error("Can't find init argument " << init_argument << "!");
    }

    const char *operator[](int idx) const
    {
        if (idx < 0 || idx >= m_other_arguments.size())
            raise_error("Trying to access other arguments with invalid index!");
        return m_other_arguments[idx];
    }

    void load_arguments(int argc, char *argv[])
    {
        // will change once an init argument is found
        VectorArgument *current_vector_argument = nullptr;
        for (int idx = 0; idx < argc; idx++)
        {
            // is init argument
            if (argv[idx][0] == '-')
            {
                bool found = false;
                // find used vector argument
                for (VectorArgument &vector_argument : m_vector_arguments)
                {
                    // update pointer if found
                    if (vector_argument.contains_init_argument(argv[idx]))
                    {
                        current_vector_argument = &vector_argument;
                        vector_argument.got_found();
                        found = true;
                        break;
                    }
                }
                if (!found)
                    raise_error("Unknown Parameter: " << argv[idx]);
            }
            // is other argument
            else
            {
                if (!current_vector_argument)
                    m_other_arguments.push_back(argv[idx]);
                else
                {
                    current_vector_argument->add_argument(argv[idx]);
                    // when enough found
                    if (current_vector_argument->is_satisfied())
                        current_vector_argument = nullptr;
                }
            }
        }
        all_required_found();
    }
};