#pragma once
#include <cmath>
#include <memory>
#include <random>
#include <iostream>
#include <sstream>

class SignalGenerator {
public:
    virtual ~SignalGenerator() = default;
    virtual double generate(int t) = 0;
    virtual std::string serialize() const = 0;
};

class BaseSignal : public SignalGenerator {
public:
    double generate(int t) override {
        return 1.0; // The original simulation expected a step response of 1.0 without decorators
    }
    std::string serialize() const override {
        return "BaseSignal\n";
    }
};

class SignalDecorator : public SignalGenerator {
protected:
    std::shared_ptr<SignalGenerator> component;
public:
    explicit SignalDecorator(std::shared_ptr<SignalGenerator> comp) : component(comp) {}
    double generate(int t) override {
        return component ? component->generate(t) : 1.0;
    }
    std::string serialize() const override {
        if(component) return component->serialize();
        return "BaseSignal\n";
    }
};

class ConstantSignal : public SignalDecorator {
    double level;
public:
    ConstantSignal(std::shared_ptr<SignalGenerator> comp, double l) : SignalDecorator(comp), level(l) {}
    double generate(int t) override {
        return SignalDecorator::generate(t) + level;
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "ConstantSignal " << level << "\n";
        return oss.str();
    }
};

class SineSignal : public SignalDecorator {
    double amplitude;
    int period;
public:
    SineSignal(std::shared_ptr<SignalGenerator> comp, double a, int p) : SignalDecorator(comp), amplitude(a), period(p) {}
    double generate(int t) override {
        if (period == 0) return SignalDecorator::generate(t);
        return SignalDecorator::generate(t) + amplitude * std::sin((static_cast<double>(t % period) / period) * 6.2831853);
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "SineSignal " << amplitude << " " << period << "\n";
        return oss.str();
    }
};

class SquareSignal : public SignalDecorator {
    double amplitude;
    int period;
    double duty_cycle;
public:
    SquareSignal(std::shared_ptr<SignalGenerator> comp, double a, int p, double dc) : SignalDecorator(comp), amplitude(a), period(p), duty_cycle(dc) {}
    double generate(int t) override {
        if (period == 0) return SignalDecorator::generate(t);
        return SignalDecorator::generate(t) + (((t % period) < (duty_cycle * period)) ? amplitude : 0);
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "SquareSignal " << amplitude << " " << period << " " << duty_cycle << "\n";
        return oss.str();
    }
};

class TriangleSignal : public SignalDecorator {
    double amplitude;
    int period;
    double duty_cycle;
public:
    TriangleSignal(std::shared_ptr<SignalGenerator> comp, double a, int p, double dc) : SignalDecorator(comp), amplitude(a), period(p), duty_cycle(dc) {}
    double generate(int t) override {
        if (period == 0) return SignalDecorator::generate(t);
        double phase = static_cast<double>(t % period) / period;
        double val = 0.0;
        if (duty_cycle > 0.0 && phase < duty_cycle) {
            val = amplitude * (phase / duty_cycle);
        } else if (duty_cycle < 1.0) {
            val = amplitude * (1.0 - (phase - duty_cycle) / (1.0 - duty_cycle));
        }
        return SignalDecorator::generate(t) + val;
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "TriangleSignal " << amplitude << " " << period << " " << duty_cycle << "\n";
        return oss.str();
    }
};

class WhiteNoiseSignal : public SignalDecorator {
    double limit;
public:
    WhiteNoiseSignal(std::shared_ptr<SignalGenerator> comp, double lim) 
        : SignalDecorator(comp), limit(lim) {}
    double generate(int t) override {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<double> dist(-limit, limit);
        return SignalDecorator::generate(t) + dist(gen);
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "WhiteNoiseSignal " << limit << "\n";
        return oss.str();
    }
};

class SaturationDecorator : public SignalDecorator {
    double limit;
public:
    SaturationDecorator(std::shared_ptr<SignalGenerator> comp, double l) : SignalDecorator(comp), limit(l) {}
    double generate(int t) override {
        double val = SignalDecorator::generate(t);
        if (val > limit) return limit;
        if (val < -limit) return -limit;
        return val;
    }
    std::string serialize() const override {
        std::ostringstream oss;
        oss << SignalDecorator::serialize();
        oss << "SaturationDecorator " << limit << "\n";
        return oss.str();
    }
};

class SignalSerializer {
public:
    static std::shared_ptr<SignalGenerator> deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::shared_ptr<SignalGenerator> current = std::make_shared<BaseSignal>();
        
        while (iss >> token) {
            if (token == "ConstantSignal") {
                double level;
                if (iss >> level) current = std::make_shared<ConstantSignal>(current, level);
            } else if (token == "SineSignal") {
                double a; int p;
                if (iss >> a >> p) current = std::make_shared<SineSignal>(current, a, p);
            } else if (token == "SquareSignal") {
                double a; int p; double dc;
                if (iss >> a >> p >> dc) current = std::make_shared<SquareSignal>(current, a, p, dc);
            } else if (token == "TriangleSignal") {
                double a; int p; double dc;
                if (iss >> a >> p >> dc) current = std::make_shared<TriangleSignal>(current, a, p, dc);
            } else if (token == "WhiteNoiseSignal") {
                double lim;
                if (iss >> lim) current = std::make_shared<WhiteNoiseSignal>(current, lim);
            } else if (token == "SaturationDecorator") {
                double limit;
                if (iss >> limit) current = std::make_shared<SaturationDecorator>(current, limit);
            }
        }
        return current;
    }
};
