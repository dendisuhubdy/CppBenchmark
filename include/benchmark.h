//
// Created by Ivan Shynkarenka on 01.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "context.h"
#include "phase_core.h"
#include "settings.h"

namespace CppBenchmark {

class Benchmark : public Phase
{
    friend class Launcher;

public:
    explicit Benchmark(const std::string& name, const Settings& settings = Settings::Default)
            : _name(name),
              _settings(settings)
    {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

    // Implementation of Phase
    const std::string& name() const override
    { return _current->name(); }
    const PhaseMetrics& metrics() const override
    { return _current->metrics(); }
    std::shared_ptr<Phase> Start(const std::string& phase) override
    { return _current->Start(phase); }
    void Stop() override
    { _current->Stop(); }
    std::shared_ptr<PhaseScope> Scope(const std::string& phase) override
    { return _current->Scope(phase); }

protected:
    virtual void Initialize() {}
    virtual void Run(const Context& context) = 0;
    virtual void Cleanup() {}

private:
    std::string _name;
    Settings _settings;
    std::shared_ptr<PhaseCore> _current;
    std::vector<std::shared_ptr<PhaseCore>> _benchmarks;

    std::shared_ptr<PhaseCore> UpdateCurrentBenchmark(const Context& context);

    void Launch(std::function<void (const Benchmark&, const Context&, int)> onLaunching = [](const Benchmark&, const Context&, int){},
                std::function<void (const Benchmark&, const Context&, int)> onLaunched = [](const Benchmark&, const Context&, int){});
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
