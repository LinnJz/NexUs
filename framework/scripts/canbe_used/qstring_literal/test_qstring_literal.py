import unittest
import os
import tempfile
from qstring_literal import process_content, process_file, find_cpp_files


class TestBasicStringWrapping(unittest.TestCase):
    def test_simple_string_assignment(self):
        content = 'QString str = "hello";'
        expected = 'QString str = QStringLiteral("hello");'
        self.assertEqual(process_content(content), expected)

    def test_single_string_literal(self):
        content = '"hello"'
        expected = 'QStringLiteral("hello")'
        self.assertEqual(process_content(content), expected)

    def test_string_with_braces_initialization(self):
        content = 'QString str {"hello"};'
        expected = 'QString str {QStringLiteral("hello")};'
        self.assertEqual(process_content(content), expected)


class TestFilterContexts(unittest.TestCase):
    def test_include_directive_not_wrapped(self):
        content = '#include "myfile.h"'
        expected = '#include "myfile.h"'
        self.assertEqual(process_content(content), expected)

    def test_include_directive_with_spaces(self):
        content = '#  include  "myfile.h"'
        expected = '#  include  "myfile.h"'
        self.assertEqual(process_content(content), expected)

    def test_setObjectName_not_wrapped(self):
        content = 'setObjectName("NXWidget")'
        expected = 'setObjectName("NXWidget")'
        self.assertEqual(process_content(content), expected)

    def test_setProperty_not_wrapped(self):
        content = 'setProperty("key", value)'
        expected = 'setProperty("key", value)'
        self.assertEqual(process_content(content), expected)

    def test_property_not_wrapped(self):
        content = 'property("name")'
        expected = 'property("name")'
        self.assertEqual(process_content(content), expected)

    def test_QPropertyAnimation_not_wrapped(self):
        content = 'QPropertyAnimation(this, "x")'
        expected = 'QPropertyAnimation(this, "x")'
        self.assertEqual(process_content(content), expected)

    def test_QPropertyAnimation_with_spaces(self):
        content = 'QPropertyAnimation( this , "x" )'
        expected = 'QPropertyAnimation( this , "x" )'
        self.assertEqual(process_content(content), expected)

    def test_mixed_filter_and_normal(self):
        content = 'setObjectName("w"); QString s = "hello";'
        expected = 'setObjectName("w"); QString s = QStringLiteral("hello");'
        self.assertEqual(process_content(content), expected)


class TestTrFunction(unittest.TestCase):
    def test_tr_not_wrapped(self):
        content = 'tr("hello")'
        expected = 'tr("hello")'
        self.assertEqual(process_content(content), expected)

    def test_tr_with_spaces(self):
        content = 'tr( "hello" )'
        expected = 'tr( "hello" )'
        self.assertEqual(process_content(content), expected)

    def test_tr_with_adjacent_strings(self):
        content = 'tr("hello" "world")'
        expected = 'tr("hello" "world")'
        self.assertEqual(process_content(content), expected)

    def test_tr_mixed_with_normal(self):
        content = 'tr("hello"); QString s = "world";'
        expected = 'tr("hello"); QString s = QStringLiteral("world");'
        self.assertEqual(process_content(content), expected)


class TestQStringFromFunctions(unittest.TestCase):
    def test_QString_fromUtf8_not_wrapped(self):
        content = 'QString::fromUtf8("hello")'
        expected = 'QString::fromUtf8("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromLatin1_not_wrapped(self):
        content = 'QString::fromLatin1("hello")'
        expected = 'QString::fromLatin1("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromStdString_not_wrapped(self):
        content = 'QString::fromStdString("hello")'
        expected = 'QString::fromStdString("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromLocal8Bit_not_wrapped(self):
        content = 'QString::fromLocal8Bit("hello")'
        expected = 'QString::fromLocal8Bit("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromWCharArray_not_wrapped(self):
        content = 'QString::fromWCharArray("hello")'
        expected = 'QString::fromWCharArray("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromUtf16_not_wrapped(self):
        content = 'QString::fromUtf16("hello")'
        expected = 'QString::fromUtf16("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromUcs4_not_wrapped(self):
        content = 'QString::fromUcs4("hello")'
        expected = 'QString::fromUcs4("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromCFString_not_wrapped(self):
        content = 'QString::fromCFString("hello")'
        expected = 'QString::fromCFString("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromNSString_not_wrapped(self):
        content = 'QString::fromNSString("hello")'
        expected = 'QString::fromNSString("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromRawData_not_wrapped(self):
        content = 'QString::fromRawData("hello")'
        expected = 'QString::fromRawData("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromStdU16String_not_wrapped(self):
        content = 'QString::fromStdU16String("hello")'
        expected = 'QString::fromStdU16String("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromStdU32String_not_wrapped(self):
        content = 'QString::fromStdU32String("hello")'
        expected = 'QString::fromStdU32String("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_fromStdWString_not_wrapped(self):
        content = 'QString::fromStdWString("hello")'
        expected = 'QString::fromStdWString("hello")'
        self.assertEqual(process_content(content), expected)

    def test_QString_from_with_adjacent_strings(self):
        content = 'QString::fromUtf8("hello" "world")'
        expected = 'QString::fromUtf8("hello" "world")'
        self.assertEqual(process_content(content), expected)

    def test_QString_from_mixed_with_normal(self):
        content = 'QString::fromLatin1("a"); QString s = "b";'
        expected = 'QString::fromLatin1("a"); QString s = QStringLiteral("b");'
        self.assertEqual(process_content(content), expected)


class TestGetProcAddress(unittest.TestCase):
    def test_GetProcAddress_not_wrapped(self):
        content = 'GetProcAddress(module, "RtlGetVersion")'
        expected = 'GetProcAddress(module, "RtlGetVersion")'
        self.assertEqual(process_content(content), expected)

    def test_GetProcAddress_with_spaces(self):
        content = 'GetProcAddress( module , "RtlGetVersion" )'
        expected = 'GetProcAddress( module , "RtlGetVersion" )'
        self.assertEqual(process_content(content), expected)

    def test_GetProcAddress_mixed_with_normal(self):
        content = 'GetProcAddress(h, "func"); QString s = "hello";'
        expected = 'GetProcAddress(h, "func"); QString s = QStringLiteral("hello");'
        self.assertEqual(process_content(content), expected)


class TestQtDebugMacros(unittest.TestCase):
    def test_qDebug_function_call_not_wrapped(self):
        content = 'qDebug("hello")'
        expected = 'qDebug("hello")'
        self.assertEqual(process_content(content), expected)

    def test_qWarning_function_call_not_wrapped(self):
        content = 'qWarning("hello")'
        expected = 'qWarning("hello")'
        self.assertEqual(process_content(content), expected)

    def test_qCritical_function_call_not_wrapped(self):
        content = 'qCritical("hello")'
        expected = 'qCritical("hello")'
        self.assertEqual(process_content(content), expected)

    def test_qInfo_function_call_not_wrapped(self):
        content = 'qInfo("hello")'
        expected = 'qInfo("hello")'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_not_wrapped(self):
        content = 'qDebug() << "hello";'
        expected = 'qDebug() << "hello";'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_multiple_strings(self):
        content = 'qDebug() << "a" << "b" << "c";'
        expected = 'qDebug() << "a" << "b" << "c";'
        self.assertEqual(process_content(content), expected)

    def test_qWarning_stream_not_wrapped(self):
        content = 'qWarning() << "warning";'
        expected = 'qWarning() << "warning";'
        self.assertEqual(process_content(content), expected)

    def test_qCritical_stream_not_wrapped(self):
        content = 'qCritical() << "error";'
        expected = 'qCritical() << "error";'
        self.assertEqual(process_content(content), expected)

    def test_qInfo_stream_not_wrapped(self):
        content = 'qInfo() << "info";'
        expected = 'qInfo() << "info";'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_multi_line(self):
        content = 'qDebug() << "hello"\n    << "world";'
        expected = 'qDebug() << "hello"\n    << "world";'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_adjacent_strings(self):
        content = 'qDebug() << "hello" "world";'
        expected = 'qDebug() << "hello" "world";'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_mixed_with_normal(self):
        content = 'qDebug() << "a"; QString s = "b";'
        expected = 'qDebug() << "a"; QString s = QStringLiteral("b");'
        self.assertEqual(process_content(content), expected)

    def test_qDebug_stream_with_comment_between(self):
        content = 'qDebug() /* comment */ << "hello";'
        expected = 'qDebug() /* comment */ << "hello";'
        self.assertEqual(process_content(content), expected)


class TestAlreadyWrapped(unittest.TestCase):
    def test_already_wrapped_no_double_wrap(self):
        content = 'QStringLiteral("hello")'
        expected = 'QStringLiteral("hello")'
        self.assertEqual(process_content(content), expected)

    def test_already_wrapped_with_spaces(self):
        content = 'QStringLiteral( "hello" )'
        expected = 'QStringLiteral( "hello" )'
        self.assertEqual(process_content(content), expected)

    def test_already_wrapped_in_assignment(self):
        content = 'QString str = QStringLiteral("hello");'
        expected = 'QString str = QStringLiteral("hello");'
        self.assertEqual(process_content(content), expected)

    def test_no_double_wrap_QStringLiteral_QStringLiteral(self):
        content = 'QStringLiteral(QStringLiteral("hello"))'
        expected = 'QStringLiteral(QStringLiteral("hello"))'
        self.assertEqual(process_content(content), expected)

    def test_QStringLiteral_multi_line_not_rewrapped(self):
        content = '''QStringLiteral(
    "hello"
    "world"
)'''
        expected = '''QStringLiteral(
    "hello"
    "world"
)'''
        self.assertEqual(process_content(content), expected)


class TestMultiLineAdjacentStrings(unittest.TestCase):
    def test_two_adjacent_strings_same_line(self):
        content = 'QString str = "hello" "world";'
        expected = 'QString str = QStringLiteral("hello" "world");'
        self.assertEqual(process_content(content), expected)

    def test_multi_line_adjacent_strings(self):
        content = '''QString str = "hello"
"world"
"i"
"love"
"you";'''
        expected = '''QString str = QStringLiteral("hello"
"world"
"i"
"love"
"you");'''
        self.assertEqual(process_content(content), expected)

    def test_two_lines_adjacent(self):
        content = '''QString str = "hello"
"world";'''
        expected = '''QString str = QStringLiteral("hello"
"world");'''
        self.assertEqual(process_content(content), expected)

    def test_adjacent_strings_with_comment_between(self):
        content = 'QString str = "hello" /* comment */ "world";'
        expected = 'QString str = QStringLiteral("hello" /* comment */ "world");'
        self.assertEqual(process_content(content), expected)

    def test_non_adjacent_strings_not_grouped(self):
        content = 'QString a = "hello"; QString b = "world";'
        expected = 'QString a = QStringLiteral("hello"); QString b = QStringLiteral("world");'
        self.assertEqual(process_content(content), expected)


class TestPrefixedAndRawStrings(unittest.TestCase):
    def test_u_prefixed_not_wrapped(self):
        content = 'u"hello"'
        expected = 'u"hello"'
        self.assertEqual(process_content(content), expected)

    def test_U_prefixed_not_wrapped(self):
        content = 'U"hello"'
        expected = 'U"hello"'
        self.assertEqual(process_content(content), expected)

    def test_u8_prefixed_not_wrapped(self):
        content = 'u8"hello"'
        expected = 'u8"hello"'
        self.assertEqual(process_content(content), expected)

    def test_L_prefixed_not_wrapped(self):
        content = 'L"hello"'
        expected = 'L"hello"'
        self.assertEqual(process_content(content), expected)

    def test_raw_string_not_wrapped(self):
        content = 'R"(hello world)"'
        expected = 'R"(hello world)"'
        self.assertEqual(process_content(content), expected)

    def test_raw_string_with_quotes_inside(self):
        content = 'R"(hello "world")"'
        expected = 'R"(hello "world")"'
        self.assertEqual(process_content(content), expected)

    def test_raw_string_with_delimiter(self):
        content = 'R"delim(hello)delim"'
        expected = 'R"delim(hello)delim"'
        self.assertEqual(process_content(content), expected)

    def test_uR_combined_not_wrapped(self):
        content = 'uR"(hello)"'
        expected = 'uR"(hello)"'
        self.assertEqual(process_content(content), expected)

    def test_UR_combined_not_wrapped(self):
        content = 'UR"(hello)"'
        expected = 'UR"(hello)"'
        self.assertEqual(process_content(content), expected)

    def test_LR_combined_not_wrapped(self):
        content = 'LR"(hello)"'
        expected = 'LR"(hello)"'
        self.assertEqual(process_content(content), expected)


class TestEscapedCharacters(unittest.TestCase):
    def test_escaped_quote_inside_string(self):
        content = r'"\"123"'
        expected = r'QStringLiteral("\"123")'
        self.assertEqual(process_content(content), expected)

    def test_escaped_quote_multi_line_adjacent(self):
        content = r'''QString str = "\"123"
"456\"";'''
        expected = r'''QString str = QStringLiteral("\"123"
"456\"");'''
        self.assertEqual(process_content(content), expected)

    def test_escaped_backslash(self):
        content = r'"hello\\world"'
        expected = r'QStringLiteral("hello\\world")'
        self.assertEqual(process_content(content), expected)

    def test_multiple_escape_sequences(self):
        content = r'"\n\t\r\"\\"'
        expected = r'QStringLiteral("\n\t\r\"\\")'
        self.assertEqual(process_content(content), expected)

    def test_empty_string(self):
        content = '""'
        expected = 'QStringLiteral("")'
        self.assertEqual(process_content(content), expected)

    def test_string_with_percent_signs(self):
        content = '"%1 %2"'
        expected = 'QStringLiteral("%1 %2")'
        self.assertEqual(process_content(content), expected)


class TestStringConcatenation(unittest.TestCase):
    def test_adjacent_quotes_concatenation(self):
        content = '"a""b"'
        expected = 'QStringLiteral("a""b")'
        self.assertEqual(process_content(content), expected)

    def test_brace_init_with_concatenation(self):
        content = 'QString str {"a""b"};'
        expected = 'QString str {QStringLiteral("a""b")};'
        self.assertEqual(process_content(content), expected)

    def test_triple_concatenation(self):
        content = '"a""b""c"'
        expected = 'QStringLiteral("a""b""c")'
        self.assertEqual(process_content(content), expected)

    def test_concatenation_with_spaces(self):
        content = '"a" "b"'
        expected = 'QStringLiteral("a" "b")'
        self.assertEqual(process_content(content), expected)


class TestFileProcessing(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()

    def tearDown(self):
        for root, dirs, files in os.walk(self.temp_dir, topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))
        os.rmdir(self.temp_dir)

    def _write_file(self, rel_path, content):
        full_path = os.path.join(self.temp_dir, rel_path)
        os.makedirs(os.path.dirname(full_path), exist_ok=True)
        with open(full_path, 'w', encoding='utf-8') as f:
            f.write(content)
        return full_path

    def _read_file(self, rel_path):
        full_path = os.path.join(self.temp_dir, rel_path)
        with open(full_path, 'r', encoding='utf-8') as f:
            return f.read()

    def test_process_file_modifies_content(self):
        filepath = self._write_file('test.h', 'QString s = "hello";')
        result = process_file(filepath)
        self.assertTrue(result)
        self.assertEqual(self._read_file('test.h'), 'QString s = QStringLiteral("hello");')

    def test_process_file_no_change(self):
        filepath = self._write_file('test.cpp', 'int x = 42;')
        result = process_file(filepath)
        self.assertFalse(result)

    def test_find_cpp_files_filters_correctly(self):
        self._write_file('a.h', '')
        self._write_file('b.cpp', '')
        self._write_file('c.hpp', '')
        self._write_file('d.txt', '')
        self._write_file('e.py', '')
        files = find_cpp_files(self.temp_dir)
        basenames = {os.path.basename(f) for f in files}
        self.assertIn('a.h', basenames)
        self.assertIn('b.cpp', basenames)
        self.assertIn('c.hpp', basenames)
        self.assertNotIn('d.txt', basenames)
        self.assertNotIn('e.py', basenames)

    def test_find_cpp_files_recursive(self):
        self._write_file('src/a.h', '')
        self._write_file('src/sub/b.cpp', '')
        self._write_file('other/c.hpp', '')
        files = find_cpp_files(self.temp_dir)
        self.assertEqual(len(files), 3)

    def test_process_file_with_real_world_example(self):
        content = '''#include "mywidget.h"
#include <QString>

class MyWidget : public QWidget {
public:
    MyWidget() {
        setObjectName("MyWidget");
        QString title = "Hello";
        QString desc = "Line1"
        "Line2"
        "Line3";
        QPropertyAnimation* anim = new QPropertyAnimation(this, "x");
        setProperty("type", 1);
        QString raw = R"(raw "string")";
        QString already = QStringLiteral("already");
    }
};'''
        filepath = self._write_file('test.cpp', content)
        process_file(filepath)
        result = self._read_file('test.cpp')

        self.assertIn('#include "mywidget.h"', result)
        self.assertIn('setObjectName("MyWidget")', result)
        self.assertIn('QStringLiteral("Hello")', result)
        self.assertIn('QStringLiteral("Line1"', result)
        self.assertIn('"Line2"', result)
        self.assertIn('"Line3")', result)
        self.assertIn('QPropertyAnimation(this, "x")', result)
        self.assertIn('setProperty("type", 1)', result)
        self.assertIn('R"(raw "string")"', result)
        self.assertIn('QStringLiteral("already")', result)
        self.assertNotIn('QStringLiteral(QStringLiteral(', result)


if __name__ == '__main__':
    unittest.main()