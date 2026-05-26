import sys
import os
import tempfile
import shutil

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from macro_transform import (
    parse_macro_line,
    transform_macro_name,
    categorize_type,
    get_type_byte_size,
    make_sort_key,
    should_wrap_qs_set_cref,
    wrap_type_with_qs_set_cref,
    process_macros_in_content,
    scan_cpp_files,
    TARGET_MACROS,
    CPP_EXTENSIONS,
    HEADER_EXTENSIONS,
    ADD_CONST_REF_TYPES,
    TYPE_SIZE_MAP,
)


class TestParseMacroLine:
    def test_q_property_create_q_h_no_semicolon(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_H(int, BorderRadius)")
        assert result[1] == "Q_PROPERTY_CREATE_Q_H"
        assert result[2] == "int"
        assert result[3] == "BorderRadius"

    def test_q_property_create_q_h_with_semicolon(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_H(QString, Title);")
        assert result[1] == "Q_PROPERTY_CREATE_Q_H"
        assert result[2] == "QString"
        assert result[3] == "Title"

    def test_q_property_create_q_cpp(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, BorderRadius)")
        assert result[1] == "Q_PROPERTY_CREATE_Q_CPP"
        assert result[2] == "ElaAcrylicUrlCard"
        assert result[3] == "int"
        assert result[4] == "BorderRadius"

    def test_q_property_create_q_cpp_with_semicolon(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, Title);")
        assert result[1] == "Q_PROPERTY_CREATE_Q_CPP"
        assert result[2] == "ElaAcrylicUrlCard"
        assert result[3] == "QString"
        assert result[4] == "Title"

    def test_q_property_create_d(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_D(int, BorderRadius)")
        assert result[1] == "Q_PROPERTY_CREATE_D"
        assert result[2] == "int"
        assert result[3] == "BorderRadius"

    def test_q_property_create_d_with_semicolon(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_D(QString, Title);")
        assert result[1] == "Q_PROPERTY_CREATE_D"
        assert result[2] == "QString"
        assert result[3] == "Title"

    def test_q_property_create_no_suffix(self):
        result = parse_macro_line("Q_PROPERTY_CREATE(int, NavigationViewWidth);")
        assert result[1] == "Q_PROPERTY_CREATE"
        assert result[2] == "int"
        assert result[3] == "NavigationViewWidth"

    def test_q_property_ref_create_q_cpp(self):
        result = parse_macro_line("Q_PROPERTY_REF_CREATE_Q_CPP(ElaCopyButton, QString, CopyText)")
        assert result[1] == "Q_PROPERTY_REF_CREATE_Q_CPP"
        assert result[2] == "ElaCopyButton"
        assert result[3] == "QString"
        assert result[4] == "CopyText"

    def test_q_property_create_with_tab(self):
        result = parse_macro_line("\tQ_PROPERTY_CREATE_D(int, BorderRadius)")
        assert result[0] == "\t"
        assert result[1] == "Q_PROPERTY_CREATE_D"
        assert result[2] == "int"
        assert result[3] == "BorderRadius"

    def test_q_property_create_with_namespace_type(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_H(ElaCardPixType::PixMode, CardPixMode);")
        assert result[1] == "Q_PROPERTY_CREATE_Q_H"
        assert result[2] == "ElaCardPixType::PixMode"
        assert result[3] == "CardPixMode"

    def test_q_property_create_with_q_enum_type(self):
        result = parse_macro_line("Q_PROPERTY_CREATE_Q_CPP(ElaAutoComplete, Qt::CaseSensitivity, CaseSensitivity)")
        assert result[1] == "Q_PROPERTY_CREATE_Q_CPP"
        assert result[2] == "ElaAutoComplete"
        assert result[3] == "Qt::CaseSensitivity"
        assert result[4] == "CaseSensitivity"

    def test_non_macro_line_returns_none(self):
        result = parse_macro_line("    int _borderRadius;")
        assert result is None

    def test_q_object_line_returns_none(self):
        result = parse_macro_line("    Q_OBJECT")
        assert result is None

    def test_whitespace_preservation(self):
        result = parse_macro_line("    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)")
        assert result[0] == "    "
        assert result[1] == "Q_PROPERTY_CREATE_Q_H"
        assert result[2] == "int"
        assert result[3] == "BorderRadius"


class TestTransformMacroName:
    def test_remove_q(self):
        assert transform_macro_name("Q_PROPERTY_CREATE_Q_H") == "Q_PROPERTY_CREATE_H"
        assert transform_macro_name("Q_PROPERTY_CREATE_Q_CPP") == "Q_PROPERTY_CREATE_CPP"
        assert transform_macro_name("Q_PRIVATE_CREATE_Q_H") == "Q_PRIVATE_CREATE_H"
        assert transform_macro_name("Q_PRIVATE_CREATE_Q_CPP") == "Q_PRIVATE_CREATE_CPP"

    def test_remove_ref(self):
        assert transform_macro_name("Q_PROPERTY_REF_CREATE") == "Q_PROPERTY_CREATE"
        assert transform_macro_name("Q_PRIVATE_REF_CREATE") == "Q_PRIVATE_CREATE"
        assert transform_macro_name("Q_PROPERTY_REF_CREATE_Q_H") == "Q_PROPERTY_CREATE_H"
        assert transform_macro_name("Q_PRIVATE_REF_CREATE_Q_H") == "Q_PRIVATE_CREATE_H"
        assert transform_macro_name("Q_PROPERTY_REF_CREATE_Q_CPP") == "Q_PROPERTY_CREATE_CPP"
        assert transform_macro_name("Q_PRIVATE_REF_CREATE_Q_CPP") == "Q_PRIVATE_CREATE_CPP"

    def test_remove_both_q_and_ref(self):
        assert transform_macro_name("Q_PROPERTY_REF_CREATE_Q_H") == "Q_PROPERTY_CREATE_H"
        assert transform_macro_name("Q_PRIVATE_REF_CREATE_Q_CPP") == "Q_PRIVATE_CREATE_CPP"

    def test_no_q_or_ref(self):
        assert transform_macro_name("Q_PROPERTY_CREATE") == "Q_PROPERTY_CREATE"
        assert transform_macro_name("Q_PROPERTY_CREATE_D") == "Q_PROPERTY_CREATE_D"
        assert transform_macro_name("Q_PRIVATE_CREATE") == "Q_PRIVATE_CREATE"


class TestCategorizeType:
    def test_primitive_bool(self):
        assert categorize_type("bool") == 0

    def test_primitive_int(self):
        assert categorize_type("int") == 0

    def test_primitive_qreal(self):
        assert categorize_type("qreal") == 0

    def test_primitive_float(self):
        assert categorize_type("float") == 0

    def test_primitive_double(self):
        assert categorize_type("double") == 0

    def test_enum_type(self):
        assert categorize_type("ElaCardPixType::PixMode") == 1

    def test_qt_enum_type(self):
        assert categorize_type("Qt::CaseSensitivity") == 1

    def test_complex_qt_type(self):
        assert categorize_type("QSize") == 2

    def test_cow_type(self):
        assert categorize_type("QString") == 2

    def test_pointer_type(self):
        assert categorize_type("QWidget*") == 1

    def test_custom_class_type(self):
        assert categorize_type("ElaThemeType::ThemeMode") == 1


class TestGetTypeByteSize:
    def test_bool(self):
        assert get_type_byte_size("bool") == 1

    def test_char(self):
        assert get_type_byte_size("char") == 1

    def test_short(self):
        assert get_type_byte_size("short") == 2

    def test_int(self):
        assert get_type_byte_size("int") == 4

    def test_float(self):
        assert get_type_byte_size("float") == 4

    def test_double(self):
        assert get_type_byte_size("double") == 8

    def test_qreal(self):
        assert get_type_byte_size("qreal") == 8

    def test_qint64(self):
        assert get_type_byte_size("qint64") == 8

    def test_qsize(self):
        assert get_type_byte_size("QSize") == 8

    def test_qpoint(self):
        assert get_type_byte_size("QPoint") == 8

    def test_qsizef(self):
        assert get_type_byte_size("QSizeF") == 16

    def test_qrect(self):
        assert get_type_byte_size("QRect") == 16

    def test_qrectf(self):
        assert get_type_byte_size("QRectF") == 32

    def test_enum_default(self):
        assert get_type_byte_size("SomeEnum") == 4

    def test_pointer_type(self):
        assert get_type_byte_size("QWidget*") == 8

    def test_namespace_type(self):
        assert get_type_byte_size("ElaCardPixType::PixMode") == 4

    def test_qcolor(self):
        assert get_type_byte_size("QColor") == 4

    def test_qimage(self):
        assert get_type_byte_size("QImage") == 8

    def test_unknown_type(self):
        assert get_type_byte_size("SomeCustomClass") == 4


class TestMakeSortKey:
    def test_primitive_int(self):
        key = make_sort_key("Q_PROPERTY_CREATE_H", "int", "BorderRadius")
        assert key[0] == 0
        assert key[1] == 4
        assert key[2] == "BorderRadius"

    def test_primitive_qreal(self):
        key = make_sort_key("Q_PROPERTY_CREATE_H", "qreal", "MainOpacity")
        assert key[0] == 0
        assert key[1] == 8
        assert key[2] == "MainOpacity"

    def test_enum_type(self):
        key = make_sort_key("Q_PROPERTY_CREATE_H", "ElaCardPixType::PixMode", "CardPixMode")
        assert key[0] == 1
        assert key[1] == 4
        assert key[2] == "CardPixMode"

    def test_complex_type(self):
        key = make_sort_key("Q_PROPERTY_CREATE_H", "QSize", "CardPixmapSize")
        assert key[0] == 2
        assert key[1] == 0
        assert key[2] == "CardPixmapSize"

    def test_cow_type(self):
        key = make_sort_key("Q_PROPERTY_CREATE_H", "QString", "Title")
        assert key[0] == 2
        assert key[1] == 0
        assert key[2] == "Title"


class TestShouldWrapQsSetCref:
    def test_cow_type_in_cow_list(self):
        assert should_wrap_qs_set_cref("QString") is True

    def test_qpixmap_in_cow_list(self):
        assert should_wrap_qs_set_cref("QPixmap") is True

    def test_qbytearray_in_cow_list(self):
        assert should_wrap_qs_set_cref("QByteArray") is True

    def test_qlist_in_cow_list(self):
        assert should_wrap_qs_set_cref("QList") is True

    def test_qimage_in_cow_list(self):
        assert should_wrap_qs_set_cref("QImage") is True

    def test_qvariant_in_cow_list(self):
        assert should_wrap_qs_set_cref("QVariant") is True

    def test_qurl_in_cow_list(self):
        assert should_wrap_qs_set_cref("QUrl") is True

    def test_qsize_not_in_cow_list(self):
        assert should_wrap_qs_set_cref("QSize") is False

    def test_int_not_in_cow_list(self):
        assert should_wrap_qs_set_cref("int") is False

    def test_qreal_not_in_cow_list(self):
        assert should_wrap_qs_set_cref("qreal") is False

    def test_enum_not_in_cow_list(self):
        assert should_wrap_qs_set_cref("ElaCardPixType::PixMode") is False


class TestWrapTypeWithQsSetCref:
    def test_wrap_qstring(self):
        assert wrap_type_with_qs_set_cref("QString") == "QS_SET_CREF(QString)"

    def test_no_wrap_for_non_cow(self):
        assert wrap_type_with_qs_set_cref("int") == "int"


SAMPLE_Q_H_INPUT = """    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(qreal, MainOpacity)
    Q_PROPERTY_CREATE_Q_H(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_Q_H(int, BrushAlpha)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
    Q_PROPERTY_CREATE_Q_H(int, SubTitleSpacing);
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaCardPixType::PixMode, CardPixMode);
    Q_PROPERTY_CREATE_Q_H(QString, Url);"""


EXPECTED_Q_H_OUTPUT = """    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, BrushAlpha)
    Q_PROPERTY_CREATE_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_H(int, SubTitlePixelSize)
    Q_PROPERTY_CREATE_H(int, SubTitleSpacing)
    Q_PROPERTY_CREATE_H(int, TitlePixelSize)
    Q_PROPERTY_CREATE_H(int, TitleSpacing)
    Q_PROPERTY_CREATE_H(qreal, MainOpacity)
    Q_PROPERTY_CREATE_H(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_H(ElaCardPixType::PixMode, CardPixMode)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QPixmap), CardPixmap)
    Q_PROPERTY_CREATE_H(QSize, CardPixmapSize)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SubTitle)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Title)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Url)"""


SAMPLE_Q_CPP_INPUT = """Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, qreal, MainOpacity)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, qreal, NoiseOpacity)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, BrushAlpha)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, SubTitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, ElaCardPixType::PixMode, CardPixMode);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, Url);"""


EXPECTED_Q_CPP_OUTPUT = """Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, BorderRadius)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, BrushAlpha)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, SubTitlePixelSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, SubTitleSpacing)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, TitlePixelSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, TitleSpacing)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, qreal, MainOpacity)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, qreal, NoiseOpacity)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, ElaCardPixType::PixMode, CardPixMode)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QS_SET_CREF(QPixmap), CardPixmap)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QSize, CardPixmapSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QS_SET_CREF(QString), SubTitle)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QS_SET_CREF(QString), Title)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QS_SET_CREF(QString), Url)"""


SAMPLE_MIXED_INPUT = """Q_PROPERTY_CREATE(int, NavigationViewWidth);
Q_PROPERTY_CREATE(int, UserButtonSpacing);"""

EXPECTED_MIXED_OUTPUT = """Q_PROPERTY_CREATE(int, NavigationViewWidth)
Q_PROPERTY_CREATE(int, UserButtonSpacing)"""


class TestProcessMacrosInContent:
    def test_q_h_processing(self):
        input_content = "class Foo {\n" + SAMPLE_Q_H_INPUT + "\npublic:"
        result = process_macros_in_content(input_content)
        expected = "class Foo {\n" + EXPECTED_Q_H_OUTPUT + "\npublic:"
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_q_cpp_processing(self):
        input_content = "#include <QWidget>\n" + SAMPLE_Q_CPP_INPUT + "\nFoo::Foo()"
        result = process_macros_in_content(input_content)
        expected = "#include <QWidget>\n" + EXPECTED_Q_CPP_OUTPUT + "\nFoo::Foo()"
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_mixed_macros(self):
        input_content = "class Bar {\n" + SAMPLE_MIXED_INPUT + "\npublic:"
        result = process_macros_in_content(input_content)
        expected = "class Bar {\n" + EXPECTED_MIXED_OUTPUT + "\npublic:"
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_no_macros_passthrough(self):
        content = "class Foo {\n    int x;\n};"
        result = process_macros_in_content(content)
        assert result == content

    def test_ref_removal_merge(self):
        input_content = """Q_PROPERTY_CREATE_Q_CPP(Foo, int, A)
Q_PROPERTY_REF_CREATE_Q_CPP(Foo, QString, B)
Q_PROPERTY_CREATE_Q_CPP(Foo, qreal, C)
Q_PROPERTY_REF_CREATE_Q_CPP(Foo, QString, D)"""
        result = process_macros_in_content(input_content)
        expected = """Q_PROPERTY_CREATE_CPP(Foo, int, A)
Q_PROPERTY_CREATE_CPP(Foo, qreal, C)
Q_PROPERTY_CREATE_CPP(Foo, QS_SET_CREF(QString), B)
Q_PROPERTY_CREATE_CPP(Foo, QS_SET_CREF(QString), D)"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_multiple_macro_blocks(self):
        input_content = """#include "a.h"
Q_PROPERTY_CREATE_Q_CPP(Foo, int, A)
Q_PROPERTY_CREATE_Q_CPP(Foo, qreal, B)

class Foo {
    Q_PROPERTY_CREATE_Q_H(int, X)
    Q_PROPERTY_CREATE_Q_H(qreal, Y)
};"""
        result = process_macros_in_content(input_content)
        expected = """#include "a.h"
Q_PROPERTY_CREATE_CPP(Foo, int, A)
Q_PROPERTY_CREATE_CPP(Foo, qreal, B)

class Foo {
    Q_PROPERTY_CREATE_H(int, X)
    Q_PROPERTY_CREATE_H(qreal, Y)
};"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"


class TestScanCppFiles:
    def test_scan_finds_cpp_files(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            os.makedirs(os.path.join(tmpdir, "subdir"))
            with open(os.path.join(tmpdir, "test.h"), "w") as f:
                f.write("// header")
            with open(os.path.join(tmpdir, "test.cpp"), "w") as f:
                f.write("// source")
            with open(os.path.join(tmpdir, "test.txt"), "w") as f:
                f.write("// text")
            with open(os.path.join(tmpdir, "subdir", "nested.mm"), "w") as f:
                f.write("// objc++")

            files = scan_cpp_files(tmpdir)
            rel_paths = [os.path.relpath(f, tmpdir) for f in files]
            assert "test.h" in rel_paths
            assert "test.cpp" in rel_paths
            assert "test.txt" not in rel_paths
            assert os.path.join("subdir", "nested.mm") in rel_paths


class TestIntegration:
    def test_full_file_transform_h(self):
        input_content = """#ifndef FOO_H
#define FOO_H

#include <QPixmap>
#include "ElaProperty.h"

class Foo {
    Q_OBJECT
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(qreal, Opacity)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QPixmap, Icon);

public:
    Foo();
};"""
        result = process_macros_in_content(input_content)
        expected = """#ifndef FOO_H
#define FOO_H

#include <QPixmap>
#include "ElaProperty.h"

class Foo {
    Q_OBJECT
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(qreal, Opacity)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QPixmap), Icon)
    Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Title)

public:
    Foo();
};"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_d_no_sorting_no_wrapping(self):
        input_content = """    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
    Q_PROPERTY_CREATE_D(qreal, Opacity)
    Q_PROPERTY_CREATE_D(qreal, Rotation)
    Q_PROPERTY_CREATE_D(QImage, Image)"""
        result = process_macros_in_content(input_content)
        expected = """    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
    Q_PROPERTY_CREATE_D(qreal, Opacity)
    Q_PROPERTY_CREATE_D(qreal, Rotation)
    Q_PROPERTY_CREATE_D(QImage, Image)"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_non_h_cpp_ref_removal(self):
        input_content = """Q_PROPERTY_REF_CREATE(Foo, int, A)
Q_PRIVATE_REF_CREATE(Foo, QString, B)"""
        result = process_macros_in_content(input_content)
        expected = """Q_PROPERTY_CREATE(Foo, int, A)
Q_PRIVATE_CREATE(Foo, QString, B)"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"

    def test_bool_before_int(self):
        input_content = """Q_PROPERTY_CREATE_Q_CPP(Foo, int, Width)
Q_PROPERTY_CREATE_Q_CPP(Foo, bool, IsVisible)
Q_PROPERTY_CREATE_Q_CPP(Foo, int, Height)"""
        result = process_macros_in_content(input_content)
        expected = """Q_PROPERTY_CREATE_CPP(Foo, bool, IsVisible)
Q_PROPERTY_CREATE_CPP(Foo, int, Height)
Q_PROPERTY_CREATE_CPP(Foo, int, Width)"""
        assert result == expected, f"\nExpected:\n{expected}\n\nGot:\n{result}"


if __name__ == "__main__":
    import pytest
    sys.exit(pytest.main([__file__, "-v"]))