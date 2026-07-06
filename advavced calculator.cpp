#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <future>
#include <stdexcept>
#include <cctype>
#include <utility>
using namespace std;

// ===================== ANSI Color Codes =====================
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string DIM     = "\033[2m";
const string ITALIC  = "\033[3m";
const string ULINE   = "\033[4m";

const string RED     = "\033[91m";
const string GREEN   = "\033[92m";
const string YELLOW  = "\033[93m";
const string BLUE    = "\033[94m";
const string MAGENTA = "\033[95m";
const string CYAN    = "\033[96m";
const string WHITE   = "\033[97m";

// ===================== Constants =====================
const double PI = 3.14159265358979323846;
const double E_CONST  = 2.71828182845904523536;
const double GOLDEN_RATIO = 1.61803398874989484820;

// ===================== History =====================
struct HistoryEntry {
    string expression;
    double result;
};

vector<HistoryEntry> history;
double lastAnswer = 0.0;

// ===================== Utility Functions =====================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string formatNumber(double num) {
    if (num == static_cast<long long>(num) && abs(num) < 1e15) {
        ostringstream oss;
        oss << fixed << setprecision(0) << num;
        return oss.str();
    }
    ostringstream oss;
    oss << setprecision(10) << num;
    string s = oss.str();
    if (s.find('.') != string::npos) {
        size_t last = s.find_last_not_of('0');
        if (s[last] == '.') last--;
        s = s.substr(0, last + 1);
    }
    return s;
}

void addToHistory(const string& expr, double result) {
    HistoryEntry entry;
    entry.expression = expr;
    entry.result = result;
    history.push_back(entry);
    lastAnswer = result;
}

// ===================== UI Drawing Functions =====================
void drawLine(char ch = '-', int len = 62) {
    cout << "  " << CYAN << DIM;
    for (int i = 0; i < len; i++) cout << ch;
    cout << RESET << "\n";
}

void drawDoubleLine(int len = 62) {
    cout << "  " << CYAN << BOLD;
    for (int i = 0; i < len; i++) cout << "=";
    cout << RESET << "\n";
}

void drawHeader() {
    cout << "\n";
    drawDoubleLine();
    cout << "  " << BOLD << CYAN
         << "   ____      _            _       _                    \n"
         << "  " << BOLD << CYAN
         << "  / ___|__ _| | ___ _   _| | __ _| |_ ___  _ __       \n"
         << "  " << BOLD << CYAN
         << " | |   / _` | |/ __| | | | |/ _` | __/ _ \\| '__|      \n"
         << "  " << BOLD << MAGENTA
         << " | |__| (_| | | (__| |_| | | (_| | || (_) | |         \n"
         << "  " << BOLD << MAGENTA
         << "  \\____\\__,_|_|\\___|\\__,_|_|\\__,_|\\__\\___/|_|         \n"
         << "  " << BOLD << YELLOW
         << "    S C I E N T I F I C   &   U N I T S   v3.0        \n";
    drawDoubleLine();
    cout << "  " << DIM << WHITE << "  Ans = " << GREEN << formatNumber(lastAnswer) << RESET
         << DIM << WHITE << "  |  History: " << YELLOW << history.size() << " entries" << RESET << "\n";
    drawLine('~');
}

void drawMenuOption(int num, const string& symbol, const string& desc, const string& color = WHITE) {
    cout << "  " << color << BOLD << "  [" << setw(2) << num << "] "
         << RESET << YELLOW << setw(7) << left << symbol << " "
         << RESET << WHITE << desc << RESET << "\n";
}

void drawSectionTitle(const string& title) {
    cout << "\n  " << BOLD << MAGENTA << " " << string(3, '>') << " " << title
         << " " << string(3, '<') << RESET << "\n";
    drawLine('-', 42);
}

// ===================== Input Helpers =====================
double getNumber(const string& prompt) {
    double num;
    while (true) {
        cout << "  " << CYAN << " >> " << RESET << WHITE << prompt << ": " << YELLOW;
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  " << RED << BOLD << " [!] Invalid input. Enter a valid number." << RESET << "\n";
        } else {
            cout << RESET;
            return num;
        }
    }
}

int getInteger(const string& prompt) {
    int num;
    while (true) {
        cout << "  " << CYAN << " >> " << RESET << WHITE << prompt << ": " << YELLOW;
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  " << RED << BOLD << " [!] Invalid input. Enter a valid integer." << RESET << "\n";
        } else {
            cout << RESET;
            return num;
        }
    }
}

int getMenuChoice(const string& prompt, int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << "  " << CYAN << BOLD << " >> " << RESET << WHITE << prompt << ": " << YELLOW;
        cin >> choice;
        cout << RESET;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  " << RED << BOLD << " [!] Invalid input!" << RESET << "\n";
        } else if (choice < minVal || choice > maxVal) {
            cout << "  " << RED << BOLD << " [!] Choose between " << minVal << " and " << maxVal << RESET << "\n";
        } else {
            return choice;
        }
    }
}

// Returns true if user wants to repeat
bool askRepeat() {
    char ch;
    cout << "\n  " << CYAN << " >> " << RESET << DIM << WHITE
         << "Repeat same operation? (" << GREEN << "y" << RESET << DIM << "/" << RED << "n" << RESET << DIM << "): " << YELLOW;
    cin >> ch;
    cout << RESET;
    return (ch == 'y' || ch == 'Y');
}

void pressEnter() {
    cout << "\n" << DIM << "  Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ===================== Result / Error Display =====================
void showResult(const string& expression, double result) {
    cout << "\n";
    drawLine('*', 52);
    cout << "  " << GREEN << BOLD << "  RESULT" << RESET << "\n";
    drawLine('-', 52);
    cout << "  " << WHITE << "  " << expression << "\n";
    cout << "  " << BOLD << YELLOW << "  = " << formatNumber(result) << RESET << "\n";
    drawLine('*', 52);
    addToHistory(expression, result);
}



void showError(const string& message) {
    cout << "\n";
    drawLine('!', 52);
    cout << "  " << RED << BOLD << "  ERROR: " << RESET << RED << message << RESET << "\n";
    drawLine('!', 52);
}

// ===================== Math Helpers =====================
double factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    double res = 1;
    for (int i = 2; i <= n; i++) res *= i;
    return res;
}

double combination(int n, int r) {
    if (n < 0 || r < 0 || r > n) return -1;
    return factorial(n) / (factorial(r) * factorial(n - r));
}

double permutation(int n, int r) {
    if (n < 0 || r < 0 || r > n) return -1;
    return factorial(n) / factorial(n - r);
}

double degToRad(double d) { return d * PI / 180.0; }
double radToDeg(double r) { return r * 180.0 / PI; }

// ===================== AST & Expression Parser =====================
// Tokens
enum class TokenType {
    NUMBER, PLUS, MINUS, MUL, DIV, MOD, POW, LPAREN, RPAREN, IDENTIFIER, END
};

struct Token {
    TokenType type;
    double value;
    string id;
};

class Lexer {
    string src;
    size_t pos;
public:
    Lexer(string s) : src(s), pos(0) {}
    
    Token next() {
        while (pos < src.length() && isspace(src[pos])) pos++;
        if (pos >= src.length()) return {TokenType::END, 0, ""};
        
        char c = src[pos];
        if (isdigit(c) || c == '.') {
            size_t len;
            double val = stod(src.substr(pos), &len);
            pos += len;
            return {TokenType::NUMBER, val, ""};
        }
        if (isalpha(c)) {
            string id = "";
            while (pos < src.length() && isalpha(src[pos])) {
                id += src[pos++];
            }
            return {TokenType::IDENTIFIER, 0, id};
        }
        
        pos++;
        switch(c) {
            case '+': return {TokenType::PLUS, 0, ""};
            case '-': return {TokenType::MINUS, 0, ""};
            case '*': return {TokenType::MUL, 0, ""};
            case '/': return {TokenType::DIV, 0, ""};
            case '%': return {TokenType::MOD, 0, ""};
            case '^': return {TokenType::POW, 0, ""};
            case '(': return {TokenType::LPAREN, 0, ""};
            case ')': return {TokenType::RPAREN, 0, ""};
            default: throw runtime_error(string("Unknown character: ") + c);
        }
    }
};

struct ASTNode {
    virtual double eval() const = 0;
    virtual ~ASTNode() = default;
};

struct NumberNode : ASTNode {
    double val;
    NumberNode(double v) : val(v) {}
    double eval() const override { return val; }
};

struct BinaryOpNode : ASTNode {
    char op;
    unique_ptr<ASTNode> left, right;
    BinaryOpNode(char o, unique_ptr<ASTNode> l, unique_ptr<ASTNode> r) 
        : op(o), left(move(l)), right(move(r)) {}
    
    double eval() const override {
        // Multi-threading: evaluate left and right concurrently
        auto future_left = std::async(std::launch::async, &ASTNode::eval, left.get());
        double r_val = right->eval();
        double l_val = future_left.get();
        
        switch (op) {
            case '+': return l_val + r_val;
            case '-': return l_val - r_val;
            case '*': return l_val * r_val;
            case '/': 
                if (r_val == 0) throw runtime_error("Division by zero");
                return l_val / r_val;
            case '%':
                if (r_val == 0) throw runtime_error("Modulus by zero");
                return fmod(l_val, r_val);
            case '^': return pow(l_val, r_val);
            default: throw runtime_error("Unknown operator");
        }
    }
};

struct FuncNode : ASTNode {
    string name;
    unique_ptr<ASTNode> arg;
    FuncNode(string n, unique_ptr<ASTNode> a) : name(n), arg(move(a)) {}
    
    double eval() const override {
        double v = arg->eval();
        if (name == "sin") return sin(degToRad(v));
        if (name == "cos") return cos(degToRad(v));
        if (name == "tan") return tan(degToRad(v));
        if (name == "asin") return radToDeg(asin(v));
        if (name == "acos") return radToDeg(acos(v));
        if (name == "atan") return radToDeg(atan(v));
        if (name == "sinh") return sinh(v);
        if (name == "cosh") return cosh(v);
        if (name == "tanh") return tanh(v);
        if (name == "ln") return log(v);
        if (name == "log10" || name == "log") return log10(v);
        if (name == "sqrt") return sqrt(v);
        if (name == "cbrt") return cbrt(v);
        if (name == "abs") return abs(v);
        if (name == "exp") return exp(v);
        if (name == "fact" || name == "factorial") return factorial((int)v);
        throw runtime_error("Unknown function: " + name);
    }
};

class Parser {
    Lexer lexer;
    Token curr;
    
    void advance() { curr = lexer.next(); }
    
    unique_ptr<ASTNode> parseFactor() {
        if (curr.type == TokenType::NUMBER) {
            double v = curr.value;
            advance();
            return unique_ptr<NumberNode>(new NumberNode(v));
        } else if (curr.type == TokenType::LPAREN) {
            advance();
            auto node = parseExpr();
            if (curr.type != TokenType::RPAREN) throw runtime_error("Missing closing parenthesis");
            advance();
            return node;
        } else if (curr.type == TokenType::IDENTIFIER) {
            string id = curr.id;
            advance();
            if (id == "pi") return unique_ptr<NumberNode>(new NumberNode(PI));
            if (id == "e") return unique_ptr<NumberNode>(new NumberNode(E_CONST));
            if (curr.type == TokenType::LPAREN) {
                advance();
                auto arg = parseExpr();
                if (curr.type != TokenType::RPAREN) throw runtime_error("Missing closing parenthesis in function");
                advance();
                return unique_ptr<FuncNode>(new FuncNode(id, move(arg)));
            }
            throw runtime_error("Unknown identifier or missing function arguments: " + id);
        } else if (curr.type == TokenType::MINUS) {
            advance();
            auto node = parseFactor();
            return unique_ptr<BinaryOpNode>(new BinaryOpNode('-', unique_ptr<NumberNode>(new NumberNode(0)), move(node)));
        }
        throw runtime_error("Unexpected token in factor");
    }
    
    unique_ptr<ASTNode> parsePower() {
        auto node = parseFactor();
        while (curr.type == TokenType::POW) {
            advance();
            auto right = parseFactor();
            node = unique_ptr<BinaryOpNode>(new BinaryOpNode('^', move(node), move(right)));
        }
        return node;
    }
    
    unique_ptr<ASTNode> parseTerm() {
        auto node = parsePower();
        while (curr.type == TokenType::MUL || curr.type == TokenType::DIV || curr.type == TokenType::MOD) {
            char op = curr.type == TokenType::MUL ? '*' : (curr.type == TokenType::DIV ? '/' : '%');
            advance();
            node = unique_ptr<BinaryOpNode>(new BinaryOpNode(op, move(node), parsePower()));
        }
        return node;
    }
    
    unique_ptr<ASTNode> parseExpr() {
        auto node = parseTerm();
        while (curr.type == TokenType::PLUS || curr.type == TokenType::MINUS) {
            char op = curr.type == TokenType::PLUS ? '+' : '-';
            advance();
            node = unique_ptr<BinaryOpNode>(new BinaryOpNode(op, move(node), parseTerm()));
        }
        return node;
    }
    
public:
    Parser(string s) : lexer(s) { advance(); }
    double evaluate() {
        auto tree = parseExpr();
        if (curr.type != TokenType::END) throw runtime_error("Unexpected extra tokens");
        return tree->eval();
    }
};

void evaluateExpressionUI() {
    clearScreen();
    cout << "\n";
    drawDoubleLine();
    cout << "  " << BOLD << CYAN << "       MULTITHREADED EXPRESSION EVALUATOR" << RESET << "\n";
    drawDoubleLine();
    cout << "  " << DIM << WHITE << "  Type an expression (e.g., 2+3*sin(30)) or 'exit'." << RESET << "\n";
    cout << "  " << DIM << WHITE << "  Multiple expressions can be separated by commas." << RESET << "\n";
    drawLine();
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
    
    while (true) {
        cout << "\n  " << CYAN << BOLD << " Expr >> " << RESET << YELLOW;
        string line;
        if (!getline(cin, line)) break;
        if (line == "exit" || line == "0" || line == "quit") break;
        if (line.empty()) continue;
        
        cout << RESET;
        
        try {
            // Split by comma for batch processing
            vector<string> expressions;
            stringstream ss(line);
            string expr;
            while (getline(ss, expr, ',')) {
                if (!expr.empty()) expressions.push_back(expr);
            }
            
            if (expressions.size() == 1) {
                Parser p(expressions[0]);
                double res = p.evaluate();
                showResult(expressions[0], res);
            } else {
                // Batch processing with threads
                cout << "\n  " << DIM << WHITE << "Evaluating " << expressions.size() << " expressions concurrently..." << RESET << "\n";
                vector<future<double>> futures;
                for (const auto& ex : expressions) {
                    futures.push_back(async(launch::async, [ex]() {
                        Parser p(ex);
                        return p.evaluate();
                    }));
                }
                for (size_t i = 0; i < expressions.size(); ++i) {
                    try {
                        double res = futures[i].get();
                        cout << "  " << GREEN << BOLD << " [" << (i+1) << "] " << RESET << WHITE << expressions[i] << " = " << YELLOW << formatNumber(res) << RESET << "\n";
                    } catch (const exception& e) {
                        cout << "  " << RED << BOLD << " [" << (i+1) << "] " << RESET << RED << expressions[i] << " -> Error: " << e.what() << RESET << "\n";
                    }
                }
            }
        } catch (const exception& e) {
            showError(e.what());
        }
    }
}

// =============================================================
//                  UNIT CONVERSION SUB-MENUS
// =============================================================

// ---------- Generic conversion helper ----------
void runConversion(const string units[], const double factors[], int count, const string& category) {
    do {
        clearScreen();
        cout << "\n";
        drawDoubleLine();
        cout << "  " << BOLD << CYAN << "  " << category << " CONVERSION" << RESET << "\n";
        drawDoubleLine();
        for (int i = 0; i < count; i++) {
            cout << "  " << YELLOW << BOLD << "  [" << setw(2) << (i + 1) << "] " << RESET << WHITE << units[i] << RESET << "\n";
        }
        drawLine();

        int from = getMenuChoice("Convert FROM (number)", 1, count);
        double val = getNumber("Enter value");

        cout << "\n";
        drawLine('*', 52);
        cout << "  " << GREEN << BOLD << "  CONVERSION RESULTS" << RESET << "\n";
        drawLine('-', 52);
        cout << "  " << WHITE << "  " << formatNumber(val) << " " << units[from - 1] << "\n";
        
        for (int to = 1; to <= count; to++) {
            if (from == to) continue;
            double result = val * (factors[from - 1] / factors[to - 1]);
            cout << "  " << BOLD << YELLOW << "  = " << formatNumber(result) << " " << units[to - 1] << RESET << "\n";
        }
        drawLine('*', 52);
        
        string expr = formatNumber(val) + " " + units[from - 1] + " -> All";
        addToHistory(expr, val);

    } while (askRepeat());
}

// Special handler for temperature (non-linear conversions)
void runTemperatureConversion() {
    const string units[] = {"Celsius (C)", "Fahrenheit (F)", "Kelvin (K)"};
    do {
        clearScreen();
        cout << "\n";
        drawDoubleLine();
        cout << "  " << BOLD << CYAN << "  TEMPERATURE CONVERSION" << RESET << "\n";
        drawDoubleLine();
        for (int i = 0; i < 3; i++) {
            cout << "  " << YELLOW << BOLD << "  [" << (i + 1) << "] " << RESET << WHITE << units[i] << RESET << "\n";
        }
        drawLine();

        int from = getMenuChoice("Convert FROM", 1, 3);
        double val = getNumber("Enter temperature");

        // Convert to Celsius first
        double celsius;
        if (from == 1)      celsius = val;
        else if (from == 2) celsius = (val - 32.0) * 5.0 / 9.0;
        else                celsius = val - 273.15;

        cout << "\n";
        drawLine('*', 52);
        cout << "  " << GREEN << BOLD << "  CONVERSION RESULTS" << RESET << "\n";
        drawLine('-', 52);
        cout << "  " << WHITE << "  " << formatNumber(val) << " " << units[from - 1] << "\n";

        for (int to = 1; to <= 3; to++) {
            if (from == to) continue;
            double result;
            if (to == 1)      result = celsius;
            else if (to == 2) result = celsius * 9.0 / 5.0 + 32.0;
            else              result = celsius + 273.15;
            
            cout << "  " << BOLD << YELLOW << "  = " << formatNumber(result) << " " << units[to - 1] << RESET << "\n";
        }
        drawLine('*', 52);
        
        string expr = formatNumber(val) + " " + units[from - 1] + " -> All";
        addToHistory(expr, val);

    } while (askRepeat());
}

// ---------- LENGTH ----------
void lengthConversion() {
    const string units[] = {
        "Millimeter (mm)", "Centimeter (cm)", "Meter (m)", "Kilometer (km)",
        "Inch (in)", "Foot (ft)", "Yard (yd)", "Mile (mi)",
        "Nautical Mile", "Micrometer (um)"
    };
    // All factors relative to 1 meter
    const double factors[] = {
        1000.0, 100.0, 1.0, 0.001,
        39.3701, 3.28084, 1.09361, 0.000621371,
        0.000539957, 1000000.0
    };
    runConversion(units, factors, 10, "LENGTH / DISTANCE");
}

// ---------- WEIGHT / MASS ----------
void weightConversion() {
    const string units[] = {
        "Milligram (mg)", "Gram (g)", "Kilogram (kg)", "Metric Ton",
        "Ounce (oz)", "Pound (lb)", "Stone (st)", "US Ton (short)",
        "UK Ton (long)"
    };
    // All factors relative to 1 kilogram
    const double factors[] = {
        1000000.0, 1000.0, 1.0, 0.001,
        35.274, 2.20462, 0.157473, 0.00110231,
        0.000984207
    };
    runConversion(units, factors, 9, "WEIGHT / MASS");
}

// ---------- TIME ----------
void timeConversion() {
    const string units[] = {
        "Millisecond (ms)", "Second (s)", "Minute (min)", "Hour (hr)",
        "Day", "Week", "Month (30 days)", "Year (365 days)",
        "Decade", "Century"
    };
    // All factors relative to 1 second
    const double factors[] = {
        1000.0, 1.0, 1.0/60.0, 1.0/3600.0,
        1.0/86400.0, 1.0/604800.0, 1.0/2592000.0, 1.0/31536000.0,
        1.0/315360000.0, 1.0/3153600000.0
    };
    runConversion(units, factors, 10, "TIME");
}

// ---------- AREA ----------
void areaConversion() {
    const string units[] = {
        "Sq Millimeter (mm2)", "Sq Centimeter (cm2)", "Sq Meter (m2)",
        "Sq Kilometer (km2)", "Hectare (ha)", "Acre",
        "Sq Foot (ft2)", "Sq Yard (yd2)", "Sq Inch (in2)", "Sq Mile (mi2)"
    };
    // All relative to 1 sq meter
    const double factors[] = {
        1000000.0, 10000.0, 1.0,
        0.000001, 0.0001, 0.000247105,
        10.7639, 1.19599, 1550.0, 3.861e-7
    };
    runConversion(units, factors, 10, "AREA");
}

// ---------- VOLUME ----------
void volumeConversion() {
    const string units[] = {
        "Milliliter (mL)", "Liter (L)", "Cubic Meter (m3)",
        "Gallon US", "Gallon UK", "Quart US",
        "Pint US", "Cup US", "Fluid Oz US",
        "Cubic Centimeter (cm3)"
    };
    // All relative to 1 liter
    const double factors[] = {
        1000.0, 1.0, 0.001,
        0.264172, 0.219969, 1.05669,
        2.11338, 4.22675, 33.814,
        1000.0
    };
    runConversion(units, factors, 10, "VOLUME");
}

// ---------- SPEED ----------
void speedConversion() {
    const string units[] = {
        "Meter/sec (m/s)", "Kilometer/hr (km/h)", "Mile/hr (mph)",
        "Knot (kn)", "Foot/sec (ft/s)", "Mach (at sea level)",
        "Speed of Light (c)"
    };
    // All relative to 1 m/s
    const double factors[] = {
        1.0, 3.6, 2.23694,
        1.94384, 3.28084, 0.00293858,
        3.3356e-9
    };
    runConversion(units, factors, 7, "SPEED");
}

// ---------- DATA STORAGE ----------
void dataConversion() {
    const string units[] = {
        "Bit", "Byte", "Kilobyte (KB)", "Megabyte (MB)",
        "Gigabyte (GB)", "Terabyte (TB)", "Petabyte (PB)",
        "Kibibyte (KiB)", "Mebibyte (MiB)", "Gibibyte (GiB)"
    };
    // All relative to 1 byte
    const double factors[] = {
        8.0, 1.0, 0.001, 0.000001,
        1e-9, 1e-12, 1e-15,
        1.0/1024.0, 1.0/1048576.0, 1.0/1073741824.0
    };
    runConversion(units, factors, 10, "DATA STORAGE");
}

// ---------- PRESSURE ----------
void pressureConversion() {
    const string units[] = {
        "Pascal (Pa)", "Kilopascal (kPa)", "Bar",
        "Atmosphere (atm)", "PSI (lb/in2)", "mmHg (Torr)",
        "inHg"
    };
    // All relative to 1 Pascal
    const double factors[] = {
        1.0, 0.001, 0.00001,
        9.8692e-6, 0.000145038, 0.00750062,
        0.000295300
    };
    runConversion(units, factors, 7, "PRESSURE");
}

// ---------- ENERGY ----------
void energyConversion() {
    const string units[] = {
        "Joule (J)", "Kilojoule (kJ)", "Calorie (cal)",
        "Kilocalorie (kcal)", "Watt-hour (Wh)", "Kilowatt-hour (kWh)",
        "Electronvolt (eV)", "BTU"
    };
    // All relative to 1 Joule
    const double factors[] = {
        1.0, 0.001, 0.239006,
        0.000239006, 0.000277778, 2.7778e-7,
        6.242e+18, 0.000947817
    };
    runConversion(units, factors, 8, "ENERGY");
}

// ---------- CURRENCY ----------
void currencyConversion() {
    const string units[] = {
        "US Dollar (USD)", "Euro (EUR)", "British Pound (GBP)",
        "Iraqi Dinar (IQD)", "Saudi Riyal (SAR)", "UAE Dirham (AED)",
        "Turkish Lira (TRY)", "Kuwaiti Dinar (KWD)", "Egyptian Pound (EGP)",
        "Jordanian Dinar (JOD)", "Japanese Yen (JPY)", "Indian Rupee (INR)",
        "Chinese Yuan (CNY)"
    };
    // Approximate rates relative to 1 USD (static rates - for reference only)
    const double toUSD[] = {
        1.0,          // USD
        1.08,         // EUR -> USD
        1.27,         // GBP -> USD
        0.000763,     // IQD -> USD
        0.2667,       // SAR -> USD
        0.2723,       // AED -> USD
        0.0294,       // TRY -> USD
        3.25,         // KWD -> USD
        0.0204,       // EGP -> USD
        1.41,         // JOD -> USD
        0.00645,      // JPY -> USD
        0.01192,      // INR -> USD
        0.1378        // CNY -> USD
    };
    const int count = 13;

    do {
        clearScreen();
        cout << "\n";
        drawDoubleLine();
        cout << "  " << BOLD << CYAN << "  CURRENCY CONVERSION" << RESET << "\n";
        cout << "  " << DIM << RED << "  (Static rates - for reference only)" << RESET << "\n";
        drawDoubleLine();
        for (int i = 0; i < count; i++) {
            cout << "  " << YELLOW << BOLD << "  [" << setw(2) << (i + 1) << "] " << RESET << WHITE << units[i] << RESET << "\n";
        }
        drawLine();

        int from = getMenuChoice("Convert FROM (number)", 1, count);
        double val = getNumber("Enter amount");

        double inUSD = val * toUSD[from - 1];
        
        cout << "\n";
        drawLine('*', 52);
        cout << "  " << GREEN << BOLD << "  CONVERSION RESULTS" << RESET << "\n";
        drawLine('-', 52);
        cout << "  " << WHITE << "  " << formatNumber(val) << " " << units[from - 1] << "\n";
        
        for (int to = 1; to <= count; to++) {
            if (from == to) continue;
            double result = inUSD / toUSD[to - 1];
            cout << "  " << BOLD << YELLOW << "  = " << formatNumber(result) << " " << units[to - 1] << RESET << "\n";
        }
        drawLine('*', 52);
        
        string expr = formatNumber(val) + " " + units[from - 1] + " -> All";
        addToHistory(expr, val);

    } while (askRepeat());
}

// ---------- CONVERSIONS SUB-MENU ----------
void conversionsMenu() {
    do {
        clearScreen();
        cout << "\n";
        drawDoubleLine();
        cout << "  " << BOLD << CYAN << "       UNIT CONVERSIONS CENTER" << RESET << "\n";
        drawDoubleLine();

        drawMenuOption(1,  "LENGTH", "mm, cm, m, km, in, ft, mi...",    GREEN);
        drawMenuOption(2,  "WEIGHT", "mg, g, kg, ton, oz, lb...",       GREEN);
        drawMenuOption(3,  "TIME",   "ms, sec, min, hr, day, year...",  BLUE);
        drawMenuOption(4,  "TEMP",   "Celsius, Fahrenheit, Kelvin",     BLUE);
        drawMenuOption(5,  "AREA",   "m2, km2, hectare, acre, ft2...",  CYAN);
        drawMenuOption(6,  "VOLUME", "mL, L, m3, gallon, pint...",      CYAN);
        drawMenuOption(7,  "SPEED",  "m/s, km/h, mph, knot, Mach...",   MAGENTA);
        drawMenuOption(8,  "DATA",   "bit, byte, KB, MB, GB, TB...",    MAGENTA);
        drawMenuOption(9,  "PRESS",  "Pa, kPa, bar, atm, PSI...",       YELLOW);
        drawMenuOption(10, "ENERGY", "J, kJ, cal, kcal, kWh, BTU...",   YELLOW);
        drawMenuOption(11, "MONEY",  "USD, EUR, GBP, IQD, SAR...",      RED);

        cout << "\n";
        drawLine();
        cout << "  " << BOLD << RED << "  [ 0] " << RESET << WHITE << BOLD << "Back to Main Menu" << RESET << "\n";
        drawLine();

        int ch = getMenuChoice("Choose category (0-11)", 0, 11);

        switch (ch) {
            case 0:  return;
            case 1:  lengthConversion();      break;
            case 2:  weightConversion();      break;
            case 3:  timeConversion();        break;
            case 4:  runTemperatureConversion(); break;
            case 5:  areaConversion();        break;
            case 6:  volumeConversion();      break;
            case 7:  speedConversion();       break;
            case 8:  dataConversion();        break;
            case 9:  pressureConversion();    break;
            case 10: energyConversion();      break;
            case 11: currencyConversion();    break;
        }
    } while (true);
}

// ===================== History & Constants =====================
void showHistory() {
    clearScreen();
    cout << "\n";
    drawDoubleLine();
    cout << "  " << BOLD << CYAN << "           CALCULATION HISTORY" << RESET << "\n";
    drawDoubleLine();
    if (history.empty()) {
        cout << "  " << DIM << WHITE << "  No calculations yet." << RESET << "\n";
    } else {
        for (int i = (int)history.size() - 1; i >= 0; i--) {
            string color = (i % 2 == 0) ? WHITE : (DIM + WHITE);
            cout << "  " << color << "  [" << setw(3) << (i + 1) << "] "
                 << history[i].expression << " = "
                 << YELLOW << formatNumber(history[i].result) << RESET << "\n";
        }
    }
    cout << "\n  " << DIM << "  Total entries: " << history.size() << RESET << "\n";
    drawDoubleLine();
}

void showConstants() {
    clearScreen();
    cout << "\n";
    drawDoubleLine();
    cout << "  " << BOLD << CYAN << "         MATHEMATICAL CONSTANTS" << RESET << "\n";
    drawDoubleLine();
    cout << "  " << YELLOW << BOLD << "  pi       " << RESET << WHITE << "= " << setprecision(15) << PI << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  e        " << RESET << WHITE << "= " << setprecision(15) << E_CONST << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  phi      " << RESET << WHITE << "= " << setprecision(15) << GOLDEN_RATIO << RESET
         << DIM << "  (Golden Ratio)" << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  sqrt(2)  " << RESET << WHITE << "= " << setprecision(15) << sqrt(2.0) << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  sqrt(3)  " << RESET << WHITE << "= " << setprecision(15) << sqrt(3.0) << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  ln(2)    " << RESET << WHITE << "= " << setprecision(15) << log(2.0) << RESET << "\n";
    cout << "  " << YELLOW << BOLD << "  ln(10)   " << RESET << WHITE << "= " << setprecision(15) << log(10.0) << RESET << "\n";
    drawDoubleLine();
}

// ===================== Main Menu =====================
void displayMainMenu() {
    clearScreen();
    drawHeader();

    drawSectionTitle("BASIC ARITHMETIC");
    drawMenuOption(1,  "+",      "Addition",              GREEN);
    drawMenuOption(2,  "-",      "Subtraction",           GREEN);
    drawMenuOption(3,  "*",      "Multiplication",        GREEN);
    drawMenuOption(4,  "/",      "Division",              GREEN);
    drawMenuOption(5,  "%",      "Modulus (Remainder)",    GREEN);
    drawMenuOption(6,  "x^y",    "Power",                 GREEN);

    drawSectionTitle("ROOTS & LOGARITHMS");
    drawMenuOption(7,  "sqrt",   "Square Root",           BLUE);
    drawMenuOption(8,  "cbrt",   "Cube Root",             BLUE);
    drawMenuOption(9,  "nrt",    "Nth Root (x^(1/n))",    BLUE);
    drawMenuOption(10, "ln",     "Natural Logarithm",     BLUE);
    drawMenuOption(11, "log10",  "Logarithm Base 10",     BLUE);
    drawMenuOption(12, "logN",   "Logarithm Base N",      BLUE);

    drawSectionTitle("TRIGONOMETRY (Degrees)");
    drawMenuOption(13, "sin",    "Sine",                  CYAN);
    drawMenuOption(14, "cos",    "Cosine",                CYAN);
    drawMenuOption(15, "tan",    "Tangent",               CYAN);
    drawMenuOption(16, "asin",   "Arc Sine",              CYAN);
    drawMenuOption(17, "acos",   "Arc Cosine",            CYAN);
    drawMenuOption(18, "atan",   "Arc Tangent",           CYAN);

    drawSectionTitle("HYPERBOLIC FUNCTIONS");
    drawMenuOption(19, "sinh",   "Hyperbolic Sine",       MAGENTA);
    drawMenuOption(20, "cosh",   "Hyperbolic Cosine",     MAGENTA);
    drawMenuOption(21, "tanh",   "Hyperbolic Tangent",    MAGENTA);

    drawSectionTitle("SPECIAL FUNCTIONS");
    drawMenuOption(22, "n!",     "Factorial",             YELLOW);
    drawMenuOption(23, "|x|",    "Absolute Value",        YELLOW);
    drawMenuOption(24, "e^x",    "Exponential",           YELLOW);
    drawMenuOption(25, "1/x",    "Reciprocal",            YELLOW);
    drawMenuOption(26, "nCr",    "Combinations",          YELLOW);
    drawMenuOption(27, "nPr",    "Permutations",          YELLOW);

    drawSectionTitle("ANGLE CONVERSION");
    drawMenuOption(28, "D->R",   "Degrees to Radians",    RED);
    drawMenuOption(29, "R->D",   "Radians to Degrees",    RED);

    drawSectionTitle("TOOLS & CONVERTERS");
    drawMenuOption(30, "UNITS",  "Unit Conversions >>",    RED);
    drawMenuOption(31, "HIST",   "View History",           RED);
    drawMenuOption(32, "CONST",  "Constants (pi, e...)",   RED);
    drawMenuOption(33, "EXPR",   "Expression Evaluator",   RED);

    cout << "\n";
    drawLine('=');
    cout << "  " << BOLD << RED << "  [ 0]  " << RESET << WHITE << BOLD << "EXIT" << RESET << "\n";
    drawDoubleLine();
}

// ===================== MAIN =====================
int main() {
    int choice;
    double num1, num2, result;
    string expr;

#ifdef _WIN32
    system("chcp 65001 > nul");
    system("");
#endif

    while (true) {
        displayMainMenu();
        cout << "\n";
        choice = getMenuChoice("Choose operation (0-33)", 0, 33);

        if (choice == 0) {
            clearScreen();
            cout << "\n";
            drawDoubleLine();
            cout << "  " << BOLD << GREEN
                 << "   Thank you for using Scientific Calculator!\n"
                 << "  " << DIM << WHITE
                 << "   Total calculations performed: " << history.size() << "\n";
            drawDoubleLine();
            cout << "\n";
            break;
        }

        // ===== TOOLS (no repeat wrapper needed, they have their own) =====
        if (choice == 30) { conversionsMenu(); continue; }
        if (choice == 31) { showHistory(); pressEnter(); continue; }
        if (choice == 32) { showConstants(); pressEnter(); continue; }
        if (choice == 33) { evaluateExpressionUI(); continue; }

        // ===== All math operations wrapped in repeat loop =====
        do {
            cout << "\n";
            drawLine('-', 42);

            switch (choice) {
                // ========== BASIC ARITHMETIC ==========
                case 1: {
                    num1 = getNumber("Enter first number");
                    num2 = getNumber("Enter second number");
                    result = num1 + num2;
                    expr = formatNumber(num1) + " + " + formatNumber(num2);
                    showResult(expr, result);
                    break;
                }
                case 2: {
                    num1 = getNumber("Enter first number");
                    num2 = getNumber("Enter second number");
                    result = num1 - num2;
                    expr = formatNumber(num1) + " - " + formatNumber(num2);
                    showResult(expr, result);
                    break;
                }
                case 3: {
                    num1 = getNumber("Enter first number");
                    num2 = getNumber("Enter second number");
                    result = num1 * num2;
                    expr = formatNumber(num1) + " * " + formatNumber(num2);
                    showResult(expr, result);
                    break;
                }
                case 4: {
                    num1 = getNumber("Enter dividend");
                    num2 = getNumber("Enter divisor");
                    if (num2 == 0) { showError("Division by zero!"); }
                    else {
                        result = num1 / num2;
                        expr = formatNumber(num1) + " / " + formatNumber(num2);
                        showResult(expr, result);
                    }
                    break;
                }
                case 5: {
                    num1 = getNumber("Enter first number");
                    num2 = getNumber("Enter second number");
                    if (num2 == 0) { showError("Modulus by zero!"); }
                    else {
                        result = fmod(num1, num2);
                        expr = formatNumber(num1) + " % " + formatNumber(num2);
                        showResult(expr, result);
                    }
                    break;
                }
                case 6: {
                    num1 = getNumber("Enter base");
                    num2 = getNumber("Enter exponent");
                    result = pow(num1, num2);
                    expr = formatNumber(num1) + " ^ " + formatNumber(num2);
                    showResult(expr, result);
                    break;
                }

                // ========== ROOTS & LOGARITHMS ==========
                case 7: {
                    num1 = getNumber("Enter number");
                    if (num1 < 0) { showError("Square root of negative number!"); }
                    else {
                        result = sqrt(num1);
                        expr = "sqrt(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 8: {
                    num1 = getNumber("Enter number");
                    result = cbrt(num1);
                    expr = "cbrt(" + formatNumber(num1) + ")";
                    showResult(expr, result);
                    break;
                }
                case 9: {
                    num1 = getNumber("Enter number (x)");
                    num2 = getNumber("Enter root degree (n)");
                    if (num2 == 0) { showError("Root degree cannot be zero!"); }
                    else if (num1 < 0 && fmod(num2, 2) == 0) { showError("Even root of negative!"); }
                    else {
                        result = pow(abs(num1), 1.0 / num2);
                        if (num1 < 0) result = -result;
                        expr = formatNumber(num1) + "^(1/" + formatNumber(num2) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 10: {
                    num1 = getNumber("Enter number");
                    if (num1 <= 0) { showError("ln undefined for zero or negative!"); }
                    else {
                        result = log(num1);
                        expr = "ln(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 11: {
                    num1 = getNumber("Enter number");
                    if (num1 <= 0) { showError("log10 undefined for zero or negative!"); }
                    else {
                        result = log10(num1);
                        expr = "log10(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 12: {
                    num1 = getNumber("Enter number");
                    num2 = getNumber("Enter base");
                    if (num1 <= 0 || num2 <= 0 || num2 == 1) { showError("Invalid log input!"); }
                    else {
                        result = log(num1) / log(num2);
                        expr = "log_" + formatNumber(num2) + "(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }

                // ========== TRIGONOMETRY ==========
                case 13: {
                    num1 = getNumber("Enter angle in degrees");
                    result = sin(degToRad(num1));
                    if (abs(result) < 1e-15) result = 0;
                    expr = "sin(" + formatNumber(num1) + " deg)";
                    showResult(expr, result);
                    break;
                }
                case 14: {
                    num1 = getNumber("Enter angle in degrees");
                    result = cos(degToRad(num1));
                    if (abs(result) < 1e-15) result = 0;
                    expr = "cos(" + formatNumber(num1) + " deg)";
                    showResult(expr, result);
                    break;
                }
                case 15: {
                    num1 = getNumber("Enter angle in degrees");
                    double cosVal = cos(degToRad(num1));
                    if (abs(cosVal) < 1e-15) { showError("Tangent undefined at " + formatNumber(num1) + " deg!"); }
                    else {
                        result = tan(degToRad(num1));
                        if (abs(result) < 1e-15) result = 0;
                        expr = "tan(" + formatNumber(num1) + " deg)";
                        showResult(expr, result);
                    }
                    break;
                }
                case 16: {
                    num1 = getNumber("Enter value (-1 to 1)");
                    if (num1 < -1 || num1 > 1) { showError("asin only defined for [-1, 1]!"); }
                    else {
                        result = radToDeg(asin(num1));
                        expr = "asin(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 17: {
                    num1 = getNumber("Enter value (-1 to 1)");
                    if (num1 < -1 || num1 > 1) { showError("acos only defined for [-1, 1]!"); }
                    else {
                        result = radToDeg(acos(num1));
                        expr = "acos(" + formatNumber(num1) + ")";
                        showResult(expr, result);
                    }
                    break;
                }
                case 18: {
                    num1 = getNumber("Enter value");
                    result = radToDeg(atan(num1));
                    expr = "atan(" + formatNumber(num1) + ")";
                    showResult(expr, result);
                    break;
                }

                // ========== HYPERBOLIC ==========
                case 19: {
                    num1 = getNumber("Enter number");
                    result = sinh(num1);
                    expr = "sinh(" + formatNumber(num1) + ")";
                    showResult(expr, result);
                    break;
                }
                case 20: {
                    num1 = getNumber("Enter number");
                    result = cosh(num1);
                    expr = "cosh(" + formatNumber(num1) + ")";
                    showResult(expr, result);
                    break;
                }
                case 21: {
                    num1 = getNumber("Enter number");
                    result = tanh(num1);
                    expr = "tanh(" + formatNumber(num1) + ")";
                    showResult(expr, result);
                    break;
                }

                // ========== SPECIAL FUNCTIONS ==========
                case 22: {
                    int n = getInteger("Enter non-negative integer");
                    if (n < 0) { showError("Factorial not defined for negatives!"); }
                    else if (n > 170) { showError("Too large! Max is 170."); }
                    else {
                        result = factorial(n);
                        ostringstream oss; oss << n << "!";
                        showResult(oss.str(), result);
                    }
                    break;
                }
                case 23: {
                    num1 = getNumber("Enter number");
                    result = abs(num1);
                    expr = "|" + formatNumber(num1) + "|";
                    showResult(expr, result);
                    break;
                }
                case 24: {
                    num1 = getNumber("Enter exponent (x)");
                    result = exp(num1);
                    expr = "e^" + formatNumber(num1);
                    showResult(expr, result);
                    break;
                }
                case 25: {
                    num1 = getNumber("Enter number");
                    if (num1 == 0) { showError("Reciprocal of zero!"); }
                    else {
                        result = 1.0 / num1;
                        expr = "1 / " + formatNumber(num1);
                        showResult(expr, result);
                    }
                    break;
                }
                case 26: {
                    int n = getInteger("Enter n");
                    int r = getInteger("Enter r");
                    if (n < 0 || r < 0 || r > n) { showError("Invalid! Need 0 <= r <= n"); }
                    else {
                        result = combination(n, r);
                        ostringstream oss; oss << "C(" << n << "," << r << ")";
                        showResult(oss.str(), result);
                    }
                    break;
                }
                case 27: {
                    int n = getInteger("Enter n");
                    int r = getInteger("Enter r");
                    if (n < 0 || r < 0 || r > n) { showError("Invalid! Need 0 <= r <= n"); }
                    else {
                        result = permutation(n, r);
                        ostringstream oss; oss << "P(" << n << "," << r << ")";
                        showResult(oss.str(), result);
                    }
                    break;
                }

                // ========== ANGLE CONVERSION ==========
                case 28: {
                    num1 = getNumber("Enter degrees");
                    result = degToRad(num1);
                    expr = formatNumber(num1) + " deg -> rad";
                    showResult(expr, result);
                    break;
                }
                case 29: {
                    num1 = getNumber("Enter radians");
                    result = radToDeg(num1);
                    expr = formatNumber(num1) + " rad -> deg";
                    showResult(expr, result);
                    break;
                }
            }
        } while (askRepeat());

        pressEnter();
    }

    return 0;
}
