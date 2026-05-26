import time
import os
import sys
import tempfile
import statistics

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from qstring_literal import process_content, find_cpp_files, process_file


LARGE_FILE_CONTENT = '''#include "mywidget.h"
#include "other.h"
#include <QString>
#include <QWidget>
#include <QPropertyAnimation>

class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setObjectName("MyWidget");
        setProperty("type", 1);
        setProperty("mode", "advanced");
        setProperty("flag", true);

        QString title = "Hello World";
        QString desc = "This is a long description "
        "that spans multiple "
        "lines for testing "
        "purposes only";
        QString path = "C:\\\\Users\\\\test\\\\file.txt";
        QString empty = "";
        QString escaped = "\\"quoted\\" and \\\\backslash";
        QString format = "%1 %2 %3";

        QString already1 = QStringLiteral("already wrapped");
        QString already2 = QStringLiteral(
            "multi"
            "line"
            "already"
        );

        QString concat = "a""b""c""d""e";
        QString spaced = "x" "y" "z";

        QPropertyAnimation* anim = new QPropertyAnimation(this, "x");
        QPropertyAnimation* anim2 = new QPropertyAnimation(this, "y");

        QString raw = R"(raw "string" here)";
        QString raw2 = R"delim(multi
line raw)delim";
        QString u16 = u"unicode16";
        QString u32 = U"unicode32";
        QString u8s = u8"utf8";
        QString wide = L"wide string";

        QString mixed = "normal" R"(raw)" "normal2";
    }

    void someMethod() {
        QString local1 = "local string 1";
        QString local2 = "local string 2";
        QString local3 = "local string 3";
        QString local4 = "local string 4";
        QString local5 = "local string 5";
        QString local6 = "local string 6";
        QString local7 = "local string 7";
        QString local8 = "local string 8";
        QString local9 = "local string 9";
        QString local10 = "local string 10";
    }

    QString anotherMethod() {
        return "return value";
    }

private:
    QString m_name = "default";
    QString m_label = "Label";
    QString m_tooltip = "Tooltip text";
    QString m_status = "Ready";
    QString m_error = "Error occurred";
    QString m_warning = "Warning message";
    QString m_info = "Information";
    QString m_debug = "Debug output";
    QString m_trace = "Trace log";
    QString m_fatal = "Fatal error";
};
'''

def generate_large_content(num_copies):
    lines = []
    for i in range(num_copies):
        lines.append(f'// Block {i}')
        lines.append(LARGE_FILE_CONTENT)
    return '\n'.join(lines)


def benchmark_process_content(content, iterations=10):
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = process_content(content)
        elapsed = time.perf_counter() - start
        times.append(elapsed)
    return statistics.mean(times), statistics.stdev(times) if len(times) > 1 else 0


def main():
    print("=" * 60)
    print("BENCHMARK: QStringLiteral Wrapper Performance")
    print("=" * 60)

    sizes = [1, 5, 10, 50, 100, 200]
    for num_copies in sizes:
        content = generate_large_content(num_copies)
        size_kb = len(content) / 1024
        mean_time, stdev = benchmark_process_content(content, iterations=3)
        throughput = size_kb / mean_time if mean_time > 0 else 0
        print(f"  Size: {size_kb:8.1f} KB | "
              f"Time: {mean_time*1000:8.2f} ms | "
              f"Throughput: {throughput:8.1f} KB/s | "
              f"Stdev: {stdev*1000:.2f} ms")

    print()
    print("=" * 60)
    print("FILE I/O BENCHMARK")
    print("=" * 60)

    tmpdir = tempfile.mkdtemp()
    try:
        for num_copies in [1, 10, 50]:
            content = generate_large_content(num_copies)
            filepath = os.path.join(tmpdir, f'test_{num_copies}.cpp')
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)

            times = []
            for _ in range(3):
                start = time.perf_counter()
                process_file(filepath)
                elapsed = time.perf_counter() - start
                times.append(elapsed)

            mean_time = statistics.mean(times)
            size_kb = len(content) / 1024
            print(f"  File: {size_kb:8.1f} KB | "
                  f"Time: {mean_time*1000:8.2f} ms | "
                  f"Throughput: {size_kb/mean_time:8.1f} KB/s")
    finally:
        for f in os.listdir(tmpdir):
            os.remove(os.path.join(tmpdir, f))
        os.rmdir(tmpdir)

    print()
    print("Done.")


if __name__ == '__main__':
    main()