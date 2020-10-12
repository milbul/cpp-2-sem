#include "calc.h"

#include <cctype> // for std::isspace
#include <cmath> // various math functions
#include <iostream> // for error reporting via std::cerr

namespace {

    const std::size_t max_decimal_digits = 10;

    enum class Op {
        ERR, SET, ADD, SUB, MUL, DIV, REM, NEG, POW, SQRT, FOLD_ADD, FOLD_SUB, FOLD_MUL, FOLD_POW, FOLD_DIV, FOLD_REM
    };
    enum class Type {
        ERR, BINARY, UNARY, FOLD
    };

    Type op_type(const Op op) {
        switch (op) {
            // error
            case Op::ERR:
                return Type::ERR;
                // unary
            case Op::NEG:
                return Type::UNARY;
            case Op::SQRT:
                return Type::UNARY;
                // binary
            case Op::SET:
                return Type::BINARY;
            case Op::ADD:
                return Type::BINARY;
            case Op::SUB:
                return Type::BINARY;
            case Op::MUL:
                return Type::BINARY;
            case Op::DIV:
                return Type::BINARY;
            case Op::REM:
                return Type::BINARY;
            case Op::POW:
                return Type::BINARY;
            case Op::FOLD_ADD:
                return Type::FOLD;
            case Op::FOLD_SUB:
                return Type::FOLD;
            case Op::FOLD_MUL:
                return Type::FOLD;
            case Op::FOLD_POW:
                return Type::FOLD;
            case Op::FOLD_DIV:
                return Type::FOLD;
            case Op::FOLD_REM:
                return Type::FOLD;
        }
        return Type::ERR;
    }

    Op parse_op(const std::string &line, std::size_t &i) {
        const auto rollback = [&i, &line](const std::size_t n) {
            i -= n;
            std::cerr << "Unknown operation " << line << std::endl;
            return Op::ERR;
        };
        switch (line[i++]) {
            case '0':
                [[fallthrough]];
            case '1':
                [[fallthrough]];
            case '2':
                [[fallthrough]];
            case '3':
                [[fallthrough]];
            case '4':
                [[fallthrough]];
            case '5':
                [[fallthrough]];
            case '6':
                [[fallthrough]];
            case '7':
                [[fallthrough]];
            case '8':
                [[fallthrough]];
            case '9':
                --i; // a first digit is a part of op's argument
                return Op::SET;
            case '+':
                return Op::ADD;
            case '-':
                return Op::SUB;
            case '*':
                return Op::MUL;
            case '/':
                return Op::DIV;
            case '%':
                return Op::REM;
            case '_':
                return Op::NEG;
            case '^':
                return Op::POW;
            case '(':
                switch (line[i++]) {
                    case '+':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_ADD;
                            default:
                                return rollback(3);
                        }
                    case '*':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_MUL;
                            default:
                                return rollback(3);
                        }
                    case '-':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_SUB;
                            default:
                                return rollback(3);
                        }
                    case '^':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_POW;
                            default:
                                return rollback(3);
                        }
                    case '/':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_DIV;
                            default:
                                return rollback(3);
                        }
                    case '%':
                        switch (line[i++]) {
                            case ')' :
                                return Op::FOLD_REM;
                            default:
                                return rollback(3);
                        }
                    default:
                        return rollback(2);
                }
            case 'S':
                switch (line[i++]) {
                    case 'Q':
                        switch (line[i++]) {
                            case 'R':
                                switch (line[i++]) {
                                    case 'T':
                                        return Op::SQRT;
                                    default:
                                        return rollback(4);
                                }
                            default:
                                return rollback(3);
                        }
                    default:
                        return rollback(2);
                }
            default:
                return rollback(1);
        }
    }

    std::size_t skip_ws(const std::string &line, std::size_t &i) {
        while (i < line.size() && std::isspace(line[i])) {
            ++i;
        }
        return i;
    }

    double mod(const double left, const double right, const Op op) {
        if (right == 0) {
            std::cerr << "Bad right argument for remainder: " << right << std::endl;
            return left;
        }
        if (op == Op::FOLD_REM) {
            return std::fmod(left, right);
        }
        return std::remainder(left, right);
    }

    double parse_arg(const std::string &line, std::size_t &i) {
        double res = 0;
        std::size_t count = 0;
        bool good = true;
        bool integer = true;
        double fraction = 1;
        skip_ws(line, i);
        while (good && i < line.size() && count < max_decimal_digits) {
            switch (line[i]) {
                case '0':
                    [[fallthrough]];
                case '1':
                    [[fallthrough]];
                case '2':
                    [[fallthrough]];
                case '3':
                    [[fallthrough]];
                case '4':
                    [[fallthrough]];
                case '5':
                     [[fallthrough]];
                case '6':
                    [[fallthrough]];
                case '7':
                    [[fallthrough]];
                case '8':
                    [[fallthrough]];
                case '9':
                    if (integer) {
                        res *= 10;
                        res += line[i] - '0';
                    } else {
                        fraction /= 10;
                        res += (line[i] - '0') * fraction;
                    }
                    ++i;
                    ++count;
                    break;
                case '.':
                    integer = false;
                    ++i;
                    break;
                default:
                    good = false;
                    break;
            }
        }
        if ((i < line.size() && count == 0) || (count == max_decimal_digits && isdigit(line[i + 1]))) {
            std::cerr << "Argument isn't fully parsed, suffix left: '" << line.substr(i) << "'" << std::endl;
        }
        return res;
    }

    double unary(const double current, const Op op) {
        switch (op) {
            case Op::NEG:
                return -current;
            case Op::SQRT:
                if (current > 0) {
                    return std::sqrt(current);
                } else {
                    std::cerr << "Bad argument for SQRT: " << current << std::endl;
                    [[fallthrough]];
                }
            default:
                return current;
        }
    }

    double binary(const Op op, const double left, const double right) {
        switch (op) {
            case Op::SET:
                return right;
            case Op::ADD:
                return left + right;
            case Op::SUB:
                return left - right;
            case Op::MUL:
                return left * right;
            case Op::DIV:
                if (right != 0) {
                    return left / right;
                } else {
                    std::cerr << "Bad right argument for division: " << right << std::endl;
                    return left;
                }
            case Op::REM:
                return mod(left, right, op);
            case Op::POW:
                return std::pow(left, right);
            default:
                return left;
        }
    }

    Op unfold(const Op op) {
        switch (op) {
            case Op::FOLD_ADD:
                return Op::ADD;
            case Op::FOLD_DIV:
                return Op::DIV;
            case Op::FOLD_MUL:
                return Op::MUL;
            case Op::FOLD_POW:
                return Op::POW;
            case Op::FOLD_REM:
                return Op::REM;
            case Op::FOLD_SUB:
                return Op::SUB;
            default:
                return Op::ERR;
        }
    }

    double fold(const double left, const double right,const Op op) {
        if (op == Op::FOLD_REM) {
            return mod(left, right, op);
        }
        return binary(unfold(op), left, right);
    }
}

double process_line(const double current, const std::string & line)
{
    std::size_t i = 0;
    const auto op = parse_op(line, i);
    switch (op_type(op)) {
        case Type::BINARY: {
            i = skip_ws(line, i);
            const auto arg = parse_arg(line, i);
            return binary(op, current, arg);
        }
        case Type::UNARY: return unary(current, op);
        case Type::FOLD: {
            std::size_t j = i;
            auto left = current;
            auto right = parse_arg(line, i);
            while (j != i) {
                j = i;
                left = fold(left, right, op);
                right = parse_arg(line, i);
            }
            return left;
        }
        default: break;
    }
    return current;
}
