#!/usr/bin/env python3
"""
TDD tests for reorder_includes.py

Run with: python -m pytest test_reorder_includes.py -v
Or:       python test_reorder_includes.py
"""

import os
import sys
import tempfile
import shutil
import unittest
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from reorder_includes import (
    parse_includes,
    classify_includes,
    build_header_index,
    find_cpp_files,
    is_self_header,
    is_qt_include,
    resolve_prefix,
    reorder_includes_in_content,
    CppFileExtensions,
)


class TestParseIncludes(unittest.TestCase):

    def test_parse_empty_content(self):
        includes = parse_includes("")
        self.assertEqual(includes, [])

    def test_parse_no_includes(self):
        content = """int main() {
    return 0;
}"""
        includes = parse_includes(content)
        self.assertEqual(includes, [])

    def test_parse_single_angle_include(self):
        content = '#include <QApplication>\n'
        includes = parse_includes(content)
        self.assertEqual(len(includes), 1)
        self.assertEqual(includes[0].full_line, '#include <QApplication>')
        self.assertEqual(includes[0].include_type, 'angle')
        self.assertEqual(includes[0].file_path, 'QApplication')

    def test_parse_single_quote_include(self):
        content = '#include "ElaTheme.h"\n'
        includes = parse_includes(content)
        self.assertEqual(len(includes), 1)
        self.assertEqual(includes[0].full_line, '#include "ElaTheme.h"')
        self.assertEqual(includes[0].include_type, 'quote')
        self.assertEqual(includes[0].file_path, 'ElaTheme.h')

    def test_parse_mixed_includes(self):
        content = '''#include "ElaApplication.h"

#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaApplicationPrivate.h"
#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QFontInfo>
#include <QWidget>
#include <utility>
'''
        includes = parse_includes(content)
        self.assertEqual(len(includes), 10)

        quote_includes = [i for i in includes if i.include_type == 'quote']
        angle_includes = [i for i in includes if i.include_type == 'angle']
        self.assertEqual(len(quote_includes), 4)
        self.assertEqual(len(angle_includes), 6)

    def test_parse_includes_with_comments(self):
        content = '''#include <QWidget> // Qt widget
#include "ElaTheme.h" /* theme */
'''
        includes = parse_includes(content)
        self.assertEqual(len(includes), 2)
        self.assertEqual(includes[0].file_path, 'QWidget')
        self.assertEqual(includes[1].file_path, 'ElaTheme.h')

    def test_parse_includes_with_spacing(self):
        content = '''#include  <QApplication>
#include   "ElaTheme.h"
'''
        includes = parse_includes(content)
        self.assertEqual(len(includes), 2)
        self.assertEqual(includes[0].file_path, 'QApplication')
        self.assertEqual(includes[1].file_path, 'ElaTheme.h')

    def test_parse_line_numbers(self):
        content = '''// header
#include "ElaApplication.h"

#include "ElaTheme.h"
#include <QWidget>
'''
        includes = parse_includes(content)
        self.assertEqual(includes[0].line_number, 2)
        self.assertEqual(includes[1].line_number, 4)
        self.assertEqual(includes[2].line_number, 5)

    def test_parse_conditional_includes(self):
        content = '''#ifdef Q_OS_WIN
#include <d3d11.h>
#endif
'''
        includes = parse_includes(content)
        self.assertEqual(len(includes), 1)
        self.assertEqual(includes[0].file_path, 'd3d11.h')

    def test_parse_multiline_not_matched(self):
        content = '''#define FOO \\
    bar
#include <QWidget>
'''
        includes = parse_includes(content)
        self.assertEqual(len(includes), 1)


class TestIsSelfHeader(unittest.TestCase):

    def test_cpp_self_header_match(self):
        self.assertTrue(is_self_header('ElaApplication.h', 'ElaApplication.cpp'))

    def test_cpp_self_header_no_match(self):
        self.assertFalse(is_self_header('ElaTheme.h', 'ElaApplication.cpp'))

    def test_h_file_never_has_self_header(self):
        self.assertFalse(is_self_header('ElaApplication.h', 'ElaApplication.h'))

    def test_case_sensitive_match(self):
        self.assertFalse(is_self_header('elaapplication.h', 'ElaApplication.cpp'))


class TestIsQtInclude(unittest.TestCase):

    def test_qt_widget(self):
        self.assertTrue(is_qt_include('QWidget'))

    def test_qt_application(self):
        self.assertTrue(is_qt_include('QApplication'))

    def test_qt_core(self):
        self.assertTrue(is_qt_include('QObject'))

    def test_qt_math(self):
        self.assertTrue(is_qt_include('QtMath'))

    def test_qt_gui(self):
        self.assertTrue(is_qt_include('QPainter'))

    def test_not_qt_standard(self):
        self.assertFalse(is_qt_include('string'))
        self.assertFalse(is_qt_include('utility'))
        self.assertFalse(is_qt_include('cmath'))
        self.assertFalse(is_qt_include('algorithm'))

    def test_not_qt_windows(self):
        self.assertFalse(is_qt_include('d3d11.h'))
        self.assertFalse(is_qt_include('dxgi1_6.h'))


class TestBuildHeaderIndex(unittest.TestCase):

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        self.dc_dir = os.path.join(self.temp_dir, 'DeveloperComponents')
        self.cmd_dir = os.path.join(self.dc_dir, 'Command')
        self.priv_dir = os.path.join(self.temp_dir, 'private')
        os.makedirs(self.cmd_dir)
        os.makedirs(self.priv_dir)

        Path(os.path.join(self.dc_dir, 'ElaTheme.h')).touch()
        Path(os.path.join(self.dc_dir, 'ElaWinShadowHelper.h')).touch()
        Path(os.path.join(self.cmd_dir, 'ElaNavigationRouteCommand.h')).touch()
        Path(os.path.join(self.priv_dir, 'ElaApplicationPrivate.h')).touch()

    def tearDown(self):
        shutil.rmtree(self.temp_dir)

    def test_build_index(self):
        index = build_header_index(self.temp_dir)
        self.assertIn('ElaTheme.h', index)
        self.assertIn('ElaWinShadowHelper.h', index)
        self.assertIn('ElaNavigationRouteCommand.h', index)
        self.assertIn('ElaApplicationPrivate.h', index)

    def test_index_values_are_relative_paths(self):
        index = build_header_index(self.temp_dir)
        self.assertEqual(
            index['ElaTheme.h'].replace('\\', '/'),
            'DeveloperComponents/ElaTheme.h'
        )
        self.assertEqual(
            index['ElaNavigationRouteCommand.h'].replace('\\', '/'),
            'DeveloperComponents/Command/ElaNavigationRouteCommand.h'
        )
        self.assertEqual(
            index['ElaApplicationPrivate.h'].replace('\\', '/'),
            'private/ElaApplicationPrivate.h'
        )


class TestResolvePrefix(unittest.TestCase):

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        self.dc_dir = os.path.join(self.temp_dir, 'DeveloperComponents')
        self.cmd_dir = os.path.join(self.dc_dir, 'Command')
        self.priv_dir = os.path.join(self.temp_dir, 'private')
        os.makedirs(self.cmd_dir)
        os.makedirs(self.priv_dir)

        Path(os.path.join(self.dc_dir, 'ElaTheme.h')).touch()
        Path(os.path.join(self.dc_dir, 'ElaWinShadowHelper.h')).touch()
        Path(os.path.join(self.cmd_dir, 'ElaNavigationRouteCommand.h')).touch()
        Path(os.path.join(self.priv_dir, 'ElaApplicationPrivate.h')).touch()

        self.index = build_header_index(self.temp_dir)

    def tearDown(self):
        shutil.rmtree(self.temp_dir)

    def test_same_level_no_prefix(self):
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = resolve_prefix('ElaTheme.h', current_file, self.index, self.temp_dir)
        self.assertEqual(result, 'ElaTheme.h')

    def test_different_level_add_prefix(self):
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = resolve_prefix('ElaApplicationPrivate.h', current_file, self.index, self.temp_dir)
        self.assertEqual(result.replace('\\', '/'), 'private/ElaApplicationPrivate.h')

    def test_command_subdir_prefix(self):
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = resolve_prefix('ElaNavigationRouteCommand.h', current_file, self.index, self.temp_dir)
        self.assertEqual(result.replace('\\', '/'), 'DeveloperComponents/Command/ElaNavigationRouteCommand.h')

    def test_not_in_index_no_prefix(self):
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = resolve_prefix('ElaDef.h', current_file, self.index, self.temp_dir)
        self.assertEqual(result, 'ElaDef.h')

    def test_from_root_level_to_subdir(self):
        root_cpp = os.path.join(self.temp_dir, 'Main.cpp')
        Path(root_cpp).touch()
        result = resolve_prefix('ElaTheme.h', root_cpp, self.index, self.temp_dir)
        self.assertEqual(result.replace('\\', '/'), 'DeveloperComponents/ElaTheme.h')

    def test_from_subdir_to_root_level(self):
        root_header = os.path.join(self.temp_dir, 'Config.h')
        Path(root_header).touch()
        index = build_header_index(self.temp_dir)
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = resolve_prefix('Config.h', current_file, index, self.temp_dir)
        self.assertEqual(result, 'Config.h')


class TestClassifyIncludes(unittest.TestCase):

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        self.dc_dir = os.path.join(self.temp_dir, 'DeveloperComponents')
        self.priv_dir = os.path.join(self.temp_dir, 'private')
        os.makedirs(self.dc_dir)
        os.makedirs(self.priv_dir)

        Path(os.path.join(self.dc_dir, 'ElaTheme.h')).touch()
        Path(os.path.join(self.dc_dir, 'ElaWinShadowHelper.h')).touch()
        Path(os.path.join(self.priv_dir, 'ElaApplicationPrivate.h')).touch()

        self.index = build_header_index(self.temp_dir)

    def tearDown(self):
        shutil.rmtree(self.temp_dir)

    def test_classify_cpp_file(self):
        content = '''#include "ElaApplication.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaApplicationPrivate.h"
#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QFontInfo>
#include <QWidget>
#include <utility>
'''
        current_file = os.path.join(self.temp_dir, 'ElaApplication.cpp')
        parts = classify_includes(content, current_file, self.index, self.temp_dir)

        self.assertEqual(len(parts['self_header']), 1)
        self.assertEqual(parts['self_header'][0].file_path, 'ElaApplication.h')

        self.assertEqual(len(parts['standard']), 1)
        self.assertEqual(parts['standard'][0].file_path, 'utility')

        self.assertEqual(len(parts['qt']), 5)
        qt_names = [i.file_path for i in parts['qt']]
        self.assertEqual(qt_names, sorted(qt_names))

        self.assertEqual(len(parts['associated']), 3)
        assoc_names = [i.file_path for i in parts['associated']]
        self.assertEqual(assoc_names, sorted(assoc_names))

    def test_classify_h_file_no_self_header(self):
        content = '''#include <QObject>
#include <QColor>
#include "ElaDef.h"
#include "ElaTheme.h"
'''
        current_file = os.path.join(self.temp_dir, 'ElaApplicationPrivate.h')
        parts = classify_includes(content, current_file, self.index, self.temp_dir)

        self.assertEqual(len(parts['self_header']), 0)
        self.assertEqual(len(parts['standard']), 0)
        self.assertEqual(len(parts['qt']), 2)
        self.assertEqual(len(parts['associated']), 2)

    def test_associated_includes_get_resolved_prefixes(self):
        content = '''#include "ElaApplication.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaApplicationPrivate.h"
'''
        current_file = os.path.join(self.temp_dir, 'ElaApplication.cpp')
        parts = classify_includes(content, current_file, self.index, self.temp_dir)

        assoc = parts['associated']
        self.assertTrue(any('DeveloperComponents' in i.file_path for i in assoc))
        self.assertTrue(any('private' in i.file_path for i in assoc))


class TestReorderIncludesInContent(unittest.TestCase):

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        self.dc_dir = os.path.join(self.temp_dir, 'DeveloperComponents')
        self.priv_dir = os.path.join(self.temp_dir, 'private')
        os.makedirs(self.dc_dir)
        os.makedirs(self.priv_dir)

        Path(os.path.join(self.dc_dir, 'ElaTheme.h')).touch()
        Path(os.path.join(self.dc_dir, 'ElaWinShadowHelper.h')).touch()
        Path(os.path.join(self.priv_dir, 'ElaApplicationPrivate.h')).touch()

        self.index = build_header_index(self.temp_dir)

    def tearDown(self):
        shutil.rmtree(self.temp_dir)

    def test_reorder_full_example(self):
        content = '''#include "ElaApplication.h"

#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaApplicationPrivate.h"
#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QFontInfo>
#include <QWidget>
#include <utility>
'''
        current_file = os.path.join(self.temp_dir, 'ElaApplication.cpp')
        result = reorder_includes_in_content(content, current_file, self.index, self.temp_dir)

        lines = result.strip().split('\n')
        include_lines = [l for l in lines if l.startswith('#include')]

        idx_self = include_lines.index('#include "ElaApplication.h"')
        idx_utility = include_lines.index('#include <utility>')
        idx_qapp = include_lines.index('#include <QApplication>')
        idx_theme = include_lines.index('#include "DeveloperComponents/ElaTheme.h"')

        self.assertLess(idx_self, idx_utility)
        self.assertLess(idx_utility, idx_qapp)
        self.assertLess(idx_qapp, idx_theme)

    def test_reorder_preserves_non_include_content(self):
        content = '''#include "ElaCentralStackedWidget.h"

#include "ElaTheme.h"
#include <QApplication>
#include <QDebug>

int main() {
    return 0;
}
'''
        current_file = os.path.join(self.dc_dir, 'ElaCentralStackedWidget.cpp')
        result = reorder_includes_in_content(content, current_file, self.index, self.temp_dir)

        self.assertIn('int main() {', result)
        self.assertIn('return 0;', result)

    def test_reorder_preserves_preprocessor_blocks(self):
        content = '''#include "MyHeader.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <d3d11.h>
#endif

#include <QWidget>
'''
        current_file = os.path.join(self.temp_dir, 'MyFile.cpp')
        result = reorder_includes_in_content(content, current_file, self.index, self.temp_dir)

        self.assertIn('#ifdef Q_OS_WIN', result)
        self.assertIn('#include <windows.h>', result)
        self.assertIn('#include <d3d11.h>', result)
        self.assertIn('#endif', result)

    def test_reorder_h_file_no_part1(self):
        content = '''#include <QObject>

#include "ElaDef.h"
'''
        current_file = os.path.join(self.temp_dir, 'ElaSomeHeader.h')
        result = reorder_includes_in_content(content, current_file, self.index, self.temp_dir)

        lines = result.strip().split('\n')
        include_lines = [l for l in lines if l.startswith('#include')]

        self.assertEqual(include_lines[0], '#include <QObject>')
        self.assertEqual(include_lines[1], '#include "ElaDef.h"')

    def test_reorder_parts_separated_by_blank_lines(self):
        content = '''#include "ElaApplication.h"
#include <QWidget>
#include <utility>
#include "ElaTheme.h"
'''
        current_file = os.path.join(self.temp_dir, 'ElaApplication.cpp')
        result = reorder_includes_in_content(content, current_file, self.index, self.temp_dir)

        lines = result.split('\n')
        include_lines_indices = [i for i, l in enumerate(lines) if l.startswith('#include')]

        self.assertEqual(lines[include_lines_indices[0]], '#include "ElaApplication.h"')
        blank_before_standard = lines[include_lines_indices[0] + 1]
        self.assertEqual(blank_before_standard, '')

        self.assertEqual(lines[include_lines_indices[1]], '#include <utility>')
        blank_before_qt = lines[include_lines_indices[1] + 1]
        self.assertEqual(blank_before_qt, '')

        self.assertEqual(lines[include_lines_indices[2]], '#include <QWidget>')
        blank_before_assoc = lines[include_lines_indices[2] + 1]
        self.assertEqual(blank_before_assoc, '')

        self.assertIn('DeveloperComponents', lines[include_lines_indices[3]])


class TestFindCppFiles(unittest.TestCase):

    def setUp(self):
        self.temp_dir = tempfile.mkdtemp()
        Path(os.path.join(self.temp_dir, 'file1.cpp')).touch()
        Path(os.path.join(self.temp_dir, 'file2.h')).touch()
        Path(os.path.join(self.temp_dir, 'file3.hpp')).touch()
        Path(os.path.join(self.temp_dir, 'file4.txt')).touch()
        subdir = os.path.join(self.temp_dir, 'sub')
        os.makedirs(subdir)
        Path(os.path.join(subdir, 'file5.cpp')).touch()
        Path(os.path.join(subdir, 'file6.h')).touch()

    def tearDown(self):
        shutil.rmtree(self.temp_dir)

    def test_finds_cpp_files(self):
        files = find_cpp_files(self.temp_dir)
        files = [os.path.relpath(f, self.temp_dir) for f in files]
        self.assertIn('file1.cpp', files)
        self.assertIn('file2.h', files)
        self.assertIn('file3.hpp', files)
        self.assertIn(os.path.join('sub', 'file5.cpp'), files)
        self.assertIn(os.path.join('sub', 'file6.h'), files)

    def test_excludes_non_cpp_files(self):
        files = find_cpp_files(self.temp_dir)
        files = [os.path.relpath(f, self.temp_dir) for f in files]
        self.assertNotIn('file4.txt', files)


class TestNumericOrderInQtInclude(unittest.TestCase):

    def test_qt_includes_with_numbers_sorted_correctly(self):
        names = [
            'QPropertyAnimation',
            'QPainter',
            'QPainterPath',
        ]
        sorted_names = sorted(names)
        self.assertEqual(sorted_names, [
            'QPainter',
            'QPainterPath',
            'QPropertyAnimation',
        ])


if __name__ == '__main__':
    unittest.main()