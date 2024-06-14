#pragma once

#include "INCLUDE.cpp"
#include "output.hpp"
#include "utils.hpp"
#include "editor.hpp"

#include "json.hpp"
using json = nlohmann::json;

class Link {
public:
    explicit Link(std::string mode, std::string identifier) {
        this->mode = std::move(mode);
        this->identifier = std::move(identifier);
    }
    Link()
            : mode{}, identifier{}
    {
    }

    bool isEmpty() const{
        return (mode.empty() && identifier.empty());
    }

    std::string mode;
    std::string identifier;
};

class Condition {
public:
    enum Type {
        eq, // ==
        ne, // !-
        gt, // >
        ge, // >=
        lt, // <
        le, // <=
    };

    explicit Condition(Type t) {
        this->t = t;
    }

    [[nodiscard]] bool check(int l, int r) const {
        switch (this->t) {
            case Type::eq:
                return l == r;
            case Type::ne:
                return l != r;
            case Type::gt:
                return l > r;
            case Type::ge:
                return l >= r;
            case Type::lt:
                return l < r;
            case Type::le:
                return l <= r;
                // To be continued...
        }
        return false;
    }

    Type t;
};

//////////////////////////////////////////////////////////////////////////////////

using StackItem = std::variant<int, Link, Condition>;

//////////////////////////////////////////////////////////////////////////////////


class Context {
public:

    Context(std::map<std::string, int> registers,
            std::vector<StackItem> stack,
            std::map<std::string, size_t> marks
    ) {
        this->registers = std::move(registers);
        this->stack = std::move(stack);
        this->marks = std::move(marks);
    }

    std::map<std::string, int> registers;
    std::vector<StackItem> stack;
    std::map<std::string, size_t> marks;
    std::string mark_to_jump = "";

};

//////////////////////////////////////////////////////////////////////////////////

class InputCommand {};
class OutputCommand {};
class PutCommand {};

class TakeCommand {
public:
    explicit TakeCommand(const Link &b_link)
            : link { b_link }
    {
    }
    TakeCommand()
            : link{}
    {
    }
    Link link;
};

enum ArithmeticOperation {
    plus,
    minus,
    multiply,
    divide,
};

class SetMarkCommand {
public:
    explicit SetMarkCommand(std::string name) {
        this->name = std::move(name);
    }

    std::string name;
};

class JumpToMarkCommand {
public:
    explicit JumpToMarkCommand(std::string name) {
        this->name = std::move(name);
    }

    std::string name;
};
class PassCommand {};

using ConditionalBranchOperation = std::variant<
        InputCommand,
        OutputCommand,
        PutCommand,
        TakeCommand,
        ArithmeticOperation,
        SetMarkCommand,
        JumpToMarkCommand,
        PassCommand
>;

class ConditionalCommand {
public:
    ConditionalCommand(ConditionalBranchOperation true_branch, ConditionalBranchOperation false_branch) {
        this->true_branch = std::move(true_branch);
        this->false_branch = std::move(false_branch);
    }

    ConditionalBranchOperation true_branch;
    ConditionalBranchOperation false_branch;
};


struct GetCommandName {
    std::string operator()(const InputCommand& c) { return "InputCommand"; }
    std::string operator()(const OutputCommand& c) { return "OutputCommand"; }
    std::string operator()(const PutCommand& c) { return "PutCommand"; }
    std::string operator()(const TakeCommand& c) { return "TakeCommand " + c.link.mode + " " + c.link.identifier; }
    std::string operator()(const ArithmeticOperation& o) { return "Arithmetic"; }
    std::string operator()(const SetMarkCommand& c) { return "Set Mark command"; }
    std::string operator()(const ConditionalCommand& c) { return "Conditional command"; }
    std::string operator()(const Condition& c) { return "Condition"; }
    std::string operator()(const JumpToMarkCommand& c) { return "Jump to mark command"; }
    std::string operator()(const PassCommand& c) { return "Pass command"; }
    std::string operator()(const int& c) { return "Just int"; }
};

using InputData = std::variant<
        int,
        Condition,
        InputCommand,
        OutputCommand,
        PutCommand,
        TakeCommand,
        ArithmeticOperation,
        SetMarkCommand,
        ConditionalCommand,
        JumpToMarkCommand,
        PassCommand
>;

inline std::ostream& operator<<(std::ostream& os, const InputData & data) {
    return os << std::visit(GetCommandName{}, data);
}

bool not_error;

void changeError() {
    not_error = false;
};

void process_input(Context &context, InputCommand cmd, Output *output){
    int value;
    std::size_t sizeOldContents = output->getContents().length() - 1;
    std::wstring oldContents = output->getContents();

    while (output->inFocus) {
        output->input();
        output->drawFrame();
        output->draw();
    }
    
    if (output->run == true) {
    try
    {
        value = std::stoi(output->getContents().substr(sizeOldContents));
        context.stack.emplace_back(value);
    }
    catch (const std::invalid_argument & e) {
        std::wstring contents = output->getContents();
        contents.insert((int)contents.length(), L"Введено не числовое значение");
        output->setContents(contents);
        output->drawFrame();
        output->draw();
        changeError();
    }
    }
    output->inFocus = true;
}


void process_output(Context &context, OutputCommand cmd, Output *output){
    auto value = std::get<int>(context.stack.back());
    context.stack.pop_back();
    std::wstring contents = output->getContents();
    contents.insert((int)contents.length(), std::to_wstring((int)value));
    output->setContents(contents);
    output->drawFrame();
    output->draw();
}

void process_take(Context &context, TakeCommand cmd){
    if (cmd.link.isEmpty()){
        auto link = std::get<Link>(context.stack.back());
        context.stack.pop_back();
        context.stack.emplace_back(context.registers[link.identifier]);
    } else
        context.stack.emplace_back(cmd.link);
}

void process_put(Context &context, PutCommand cmd){
    auto link = std::get<Link>(context.stack.back());
    context.stack.pop_back();

    auto value = std::get<int>(context.stack.back());
    context.stack.pop_back();
    if (link.mode == "регистр") {
        context.registers[link.identifier] = value;
    }
}

void process_arithmetic(Context &context, ArithmeticOperation operation, Output *output) {
    int right = std::visit([&context](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Link>)
            return context.registers[arg.identifier];
        if constexpr (std::is_same_v<T, int>)
            return arg;
        return 0;
    }, context.stack.back());
    context.stack.pop_back();

    int left = std::visit([&context](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Link>)
            return context.registers[arg.identifier];
        if constexpr (std::is_same_v<T, int>)
            return arg;
        return 0;
    }, context.stack.back());
    context.stack.pop_back();

    // std::cout << "Left: " << left << " Right: " << right << '\n';

    int res;
    switch (operation) {
        case ArithmeticOperation::plus: {
            res = left + right;
            break;
        }
        case ArithmeticOperation::minus: {
            res = left - right;
            break;
        }
        case ArithmeticOperation::multiply: {
            res = left * right;
            break;
        }
        case ArithmeticOperation::divide: {
            if (right == 0) {
                res = 0;
                std::wstring contents = output->getContents();
                contents.insert((int)contents.length(), L"Деление на ноль");
                output->setContents(contents);
                output->drawFrame();
                output->draw();
                changeError();
            } else {
                res = left / right;
                break;
            }
        }
    }
    context.stack.emplace_back(res);
}

void process_set_mark(Context &context, const SetMarkCommand& cmd, size_t idx) {
    context.marks[cmd.name] = idx + 1;
    // std::cout << "Added " << cmd.name << " mark to context\n";
}

void process_jump_to_mark(Context &context, const JumpToMarkCommand& cmd) {
    context.mark_to_jump = cmd.name;
}

void process_condition(Context &context, Condition c) {
    context.stack.emplace_back(c);
    // std::cout << "Added condition on top of the stack\n";
}

void process_int(Context &context, int d) {
    context.stack.emplace_back(d);
    // std::cout << "Added int on top of the stack\n";
}

void process_pass(Context &context, PassCommand cmd){
    // std::cout << "PASS" << '\n';
}

ConditionalBranchOperation process_conditional(Context &context, const ConditionalCommand& cmd) {
    // std::cout << "Conditional command processing\n";
    auto condition = std::get<Condition>(context.stack.back());
    context.stack.pop_back();

    auto right_value = std::get<int>(context.stack.back());
    context.stack.pop_back();

    auto left_value = std::get<int>(context.stack.back());
    context.stack.pop_back();

    if (condition.check(left_value, right_value)) {
        // std::cout << "Returning TRUE " << '\n';
        return cmd.true_branch;
    }

    // std::cout << "Returning FALSE " << '\n';
    return cmd.false_branch;
}

//////////////////////////////////////////////////////////////////////////////////

template <class... Args>
struct variant_cast_proxy
{
    std::variant<Args...> v;

    template <class... ToArgs>
    operator std::variant<ToArgs...>() const
    {
        return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return arg ; },
                          v);
    }
};


template <class... Args>
auto variant_cast(const std::variant<Args...>& v) -> variant_cast_proxy<Args...>
{
    return {v};
}

//////////////////////////////////////////////////////////////////////////////////

std::vector<InputData> parser(std::string file_name, Output *output) {
    std::vector<InputData> program;
    std::ifstream example(file_name);
    json data;
     try
    {
        data = json::parse(example);
    }
    catch (const json::parse_error& e)
    {
         std::wstring contents = output->getContents();
        contents.insert((int)contents.length(), L"Не соответствует формату JSON");
        output->setContents(contents);
        output->drawFrame();
        output->draw();
        return program;
    }

    for (auto& elem : data) {
        if (elem.is_string()) {
            if (elem == "+")
                program.emplace_back(ArithmeticOperation::plus);
            else if (elem == "-")
                program.emplace_back(ArithmeticOperation::minus);
            else if (elem == "/")
                program.emplace_back(ArithmeticOperation::divide);
            else if (elem == "*")
                program.emplace_back(ArithmeticOperation::multiply);
            else if (elem == "ввод")
                program.emplace_back(InputCommand());
            else if (elem == "взять")
                program.emplace_back(TakeCommand());
            else if (elem == "положить")
                program.emplace_back(PutCommand());
            else if (elem == "вывод")
                program.emplace_back(OutputCommand());
            else if (elem == "=")
                program.emplace_back(Condition(Condition::Type::eq));
            else if (elem == "!=")
                program.emplace_back(Condition(Condition::Type::ne));
            else if (elem == ">")
                program.emplace_back(Condition(Condition::Type::gt));
            else if (elem == ">=")
                program.emplace_back(Condition(Condition::Type::ge));
            else if (elem == "<")
                program.emplace_back(Condition(Condition::Type::lt));
            else if (elem == "<=")
                program.emplace_back(Condition(Condition::Type::le));
            else {
                std::wstring contents = output->getContents();
                contents.insert((int)contents.length(), L"Ошибка синтаксиса");
                output->setContents(contents);
                output->drawFrame();
                output->draw();
                std::vector<InputData> bad_program;
                return bad_program; 
            }
        } else if (elem.is_object()) {
            if (elem["имя"] == "взять") {
                Link l = Link(elem["что"]["режим"], elem["что"]["имя"]);
                program.emplace_back(TakeCommand(l));
            } else if (elem["имя"] == "метка")
                program.emplace_back(SetMarkCommand(elem["метка"]));
            else if (elem["имя"] == "если") {
                if (!elem.count("нет")) {
                    program.emplace_back(ConditionalCommand(
                            JumpToMarkCommand(elem["да"]),
                            PassCommand()
                    ));
                } else {
                    program.emplace_back(ConditionalCommand(
                            JumpToMarkCommand(elem["да"]),
                            JumpToMarkCommand(elem["нет"])
                    ));
                }
                program.emplace_back(PassCommand());
            } else if (elem["имя"] == "переход")
                program.emplace_back(JumpToMarkCommand(elem["куда"]));
        } else if (elem.is_number()) {
            program.emplace_back((int)elem);
        } else {
            std::wstring contents = output->getContents();
            contents.insert((int)contents.length(), L"Ошибка синтаксиса");
            output->setContents(contents);
            output->drawFrame();
            output->draw();
            std::vector<InputData> bad_program;
            return bad_program; 
        }
    }
    return program;
};


void run(Output *output) {
    std::string name = wstringToString(output->getTitle());
    std::vector<InputData> program = parser(name, output);



    std::map<std::string, int> registers;
    std::vector<StackItem> stack;
    std::map<std::string, size_t> marks;

    auto context = std::make_shared<Context>(registers, stack, marks);
    size_t idx = 0;
    output->run = true;
    not_error = true;

    if (!program.empty()) {

    
    while (idx != program.size() && output->run && not_error) {
    if (!context->mark_to_jump.empty()) {
        if (!context->marks.count(context->mark_to_jump)) {
            auto cmd = program[idx];
            std::visit([context, idx](auto&& arg) mutable {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, SetMarkCommand>)
                    process_set_mark(*context, arg, idx);
            }, cmd);
            idx++;
            continue;
        }
        idx = context->marks[context->mark_to_jump];
        context->mark_to_jump = "";
    }
    auto cmd = program[idx];

    program = std::visit([context, program, idx, output](auto&& arg) mutable {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, InputCommand>)
            process_input(*context, arg, output);
        else if constexpr (std::is_same_v<T, OutputCommand>)
            process_output(*context, arg, output);
        else if constexpr (std::is_same_v<T, TakeCommand>)
            process_take(*context, arg);
        else if constexpr (std::is_same_v<T, PutCommand>)
            process_put(*context, arg);
        else if constexpr (std::is_same_v<T, ArithmeticOperation>)
            process_arithmetic(*context, arg, output);
        else if constexpr (std::is_same_v<T, PassCommand>)
            process_pass(*context, arg);
        else if constexpr (std::is_same_v<T, SetMarkCommand>)
            process_set_mark(*context, arg, idx);
        else if constexpr (std::is_same_v<T, Condition>)
            process_condition(*context, arg);
        else if constexpr (std::is_same_v<T, ConditionalCommand>) {
            program[idx + 1] = variant_cast(process_conditional(*context, arg));
        }
        else if constexpr (std::is_same_v<T, JumpToMarkCommand>)
            process_jump_to_mark(*context, arg);
        else if constexpr (std::is_same_v<T, int>)
            process_int(*context, arg);
        else
            {
                std::wstring contents = output->getContents();
                contents.insert((int)contents.length(), L"Ошибка синтаксиса");
                output->setContents(contents);
                output->drawFrame();
                output->draw();
                // return;
            }
        return program;
    }, cmd);

    idx++;
}
}

}