import sys
import os
import tempfile
import shutil

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import optimize_qt_params as opt


TEST_DIR = os.path.dirname(os.path.abspath(__file__))


def _read(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.read()


def _write(path, content):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)


class TestOverrideSignatureExtraction:
    def test_single_line_override_phase1(self):
        content = 'QSize sizeFromContents(int type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;'
        m = list(opt.re.finditer(r'\boverride\b', content))
        assert len(m) == 1
        sig = opt._extract_override_signature(content, m[0].start())
        assert sig is not None
        func_name, params, qual = sig.split('||')
        assert func_name == 'sizeFromContents'
        assert 'QSize' in params
        assert qual == 'const'

    def test_single_line_override_phase2(self):
        content = 'void setName(const QString &name) override;'
        m = list(opt.re.finditer(r'\boverride\b', content))
        assert len(m) == 1
        sig = opt._extract_override_signature(content, m[0].start())
        assert sig is not None
        func_name, params, qual = sig.split('||')
        assert func_name == 'setName'
        assert 'QString' in params
        assert qual == ''

    def test_east_const_override(self):
        content = 'void eastConstOverride(QSize const &sz) override;'
        m = list(opt.re.finditer(r'\boverride\b', content))
        assert len(m) == 1
        sig = opt._extract_override_signature(content, m[0].start())
        assert sig is not None
        func_name, params, qual = sig.split('||')
        assert func_name == 'eastConstOverride'
        assert 'QSize' in params
        assert qual == ''

    def test_multi_line_override_header(self):
        content = """QSize
    multiLineSize(int type,
                  const QStyleOption *option,
                  const QSize &size,
                  const QWidget *widget) const override;"""
        m = list(opt.re.finditer(r'\boverride\b', content))
        assert len(m) == 1
        sig = opt._extract_override_signature(content, m[0].start())
        assert sig is not None
        func_name, params, qual = sig.split('||')
        assert func_name == 'multiLineSize'
        assert qual == 'const'

    def test_override_with_default_value_parens(self):
        content = 'void foo(const QRect &r = QRect(0, 0, 100, 100)) const override;'
        m = list(opt.re.finditer(r'\boverride\b', content))
        assert len(m) == 1
        sig = opt._extract_override_signature(content, m[0].start())
        assert sig is not None
        func_name, params, qual = sig.split('||')
        assert func_name == 'foo'
        assert 'QRect' in params
        assert qual == 'const'


class TestCollectOverrideSignatures:
    def test_collect_from_header(self):
        hdr_path = os.path.join(TEST_DIR, 'test_override_params.h')
        content = _read(hdr_path)
        sigs = set()
        for m in opt.re.finditer(r'\boverride\b', content):
            sig = opt._extract_override_signature(content, m.start())
            if sig:
                sigs.add(sig)

        func_names = {s.split('||')[0] for s in sigs}
        assert 'sizeFromContents' in func_names
        assert 'setName' in func_names
        assert 'setPixmap' in func_names
        assert 'drawRect' in func_names
        assert 'moveTo' in func_names
        assert 'eastConstOverride' in func_names
        assert 'mixedOverride' in func_names
        assert 'setFont' in func_names
        assert 'multiLineSize' in func_names
        assert 'multiLineComplex' in func_names

        assert 'nonOverrideSize' not in func_names
        assert 'nonOverrideName' not in func_names

        print(f"  Collected {len(sigs)} override signatures: {sorted(func_names)}")


class TestSkipOverrideIntegration:
    @classmethod
    def setup_class(cls):
        cls.tmpdir = tempfile.mkdtemp(prefix='qt_opt_test_')

        cls.hdr_src = os.path.join(TEST_DIR, 'test_override_params.h')
        cls.cpp_src = os.path.join(TEST_DIR, 'test_override_params.cpp')

        cls.hdr_dst = os.path.join(cls.tmpdir, 'test_override_params.h')
        cls.cpp_dst = os.path.join(cls.tmpdir, 'test_override_params.cpp')

    @classmethod
    def teardown_class(cls):
        shutil.rmtree(cls.tmpdir, ignore_errors=True)

    def _prepare_files(self):
        shutil.copy2(self.hdr_src, self.hdr_dst)
        shutil.copy2(self.cpp_src, self.cpp_dst)

    def _run_optimize(self, skip_override):
        opt.SKIP_OVERRIDE = skip_override
        opt._override_signatures.clear()

        hdr_files = [self.hdr_dst]
        if skip_override:
            opt.collect_override_signatures(hdr_files)

        opt.process_file(self.hdr_dst)
        opt.process_file(self.cpp_dst)

    def test_skip_override_true_header(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.hdr_dst)

        assert 'const QSize &size' in content, (
            "Override Phase 1: const QSize & should remain unchanged"
        )
        assert 'const QString &name' in content, (
            "Override Phase 2: const QString & should remain unchanged"
        )
        assert 'const QPixmap &pm' in content, (
            "Override Phase 2: const QPixmap & should remain unchanged"
        )
        assert 'const QRect &r' in content, (
            "Override Phase 1: const QRect & should remain unchanged"
        )
        assert 'const QPointF &pt' in content, (
            "Override Phase 1: const QPointF & should remain unchanged"
        )
        assert 'QSize const &sz' in content, (
            "Override east const: QSize const & should remain unchanged"
        )
        assert 'const QFont &font' in content, (
            "Override Phase 2: const QFont & should remain unchanged"
        )

        assert 'QSize nonOverrideSize(const QSize &size)' in content or \
               'QSize nonOverrideSize(QSize size)' in content

        print("  SKIP_OVERRIDE=True header: PASS")

    def test_skip_override_true_cpp(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.cpp_dst)

        assert 'const QSize &size' in content, (
            "Override impl: const QSize & should remain unchanged"
        )
        assert 'const QString &name' in content, (
            "Override impl: const QString & should remain unchanged"
        )
        assert 'const QPixmap &pm' in content, (
            "Override impl: const QPixmap & should remain unchanged"
        )
        assert 'const QRect &r' in content, (
            "Override impl: const QRect & should remain unchanged"
        )
        assert 'const QPointF &pt' in content, (
            "Override impl: const QPointF & should remain unchanged"
        )
        assert 'QSize const &sz' in content, (
            "Override impl east const: QSize const & should remain unchanged"
        )
        assert 'const QFont &font' in content, (
            "Override impl: const QFont & should remain unchanged"
        )

        print("  SKIP_OVERRIDE=True cpp: PASS")

    def test_non_override_still_modified_header(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.hdr_dst)

        assert 'nonOverrideSize(' in content
        assert 'nonOverrideName(' in content
        assert 'nonOverridePixmap(' in content
        assert 'nonOverrideRect(' in content
        assert 'nonOverrideMixed(' in content

        print("  Non-override still modified header: PASS")

    def test_non_override_still_modified_cpp(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.cpp_dst)

        qsize_count = content.count('const QSize &')
        assert qsize_count >= 5, (
            f"Expected at least 5 'const QSize &' (override ones), got {qsize_count}"
        )

        print("  Non-override still modified cpp: PASS")

    def test_no_skip_override_header(self):
        self._prepare_files()
        self._run_optimize(skip_override=False)

        content = _read(self.hdr_dst)

        assert 'const QSize &size' not in content, (
            "SKIP_OVERRIDE=False: const QSize & should be changed to QSize size"
        )
        assert 'const QRect &r' not in content, (
            "SKIP_OVERRIDE=False: const QRect & should be changed to QRect r"
        )
        assert 'const QPointF &pt' not in content, (
            "SKIP_OVERRIDE=False: const QPointF & should be changed to QPointF pt"
        )
        assert 'QSize const &sz' not in content, (
            "SKIP_OVERRIDE=False: QSize const & should be changed to QSize sz"
        )

        print("  SKIP_OVERRIDE=False header: PASS")

    def test_no_skip_override_cpp(self):
        self._prepare_files()
        self._run_optimize(skip_override=False)

        content = _read(self.cpp_dst)

        assert 'const QSize &size' not in content, (
            "SKIP_OVERRIDE=False cpp: const QSize & should be changed"
        )
        assert 'const QRect &r' not in content, (
            "SKIP_OVERRIDE=False cpp: const QRect & should be changed"
        )
        assert 'const QPointF &pt' not in content, (
            "SKIP_OVERRIDE=False cpp: const QPointF & should be changed"
        )
        assert 'QSize const &sz' not in content, (
            "SKIP_OVERRIDE=False cpp: QSize const & should be changed"
        )

        print("  SKIP_OVERRIDE=False cpp: PASS")

    def test_multi_line_override_protected_header(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.hdr_dst)

        assert 'const QSize &size' in content
        assert 'const QString &name' in content
        assert 'const QPixmap &pm' in content

        print("  Multi-line override protected header: PASS")

    def test_multi_line_override_protected_cpp(self):
        self._prepare_files()
        self._run_optimize(skip_override=True)

        content = _read(self.cpp_dst)

        assert 'const QSize &size' in content
        assert 'const QString &name' in content
        assert 'const QPixmap &pm' in content

        print("  Multi-line override protected cpp: PASS")


class TestFindOverrideRegions:
    def test_header_regions_single_line(self):
        content = 'void drawRect(const QRect &r) const override;'
        regions = opt._find_override_regions_in_header(content)
        assert len(regions) == 1
        start, end = regions[0]
        assert 'drawRect' in content[start:end]
        assert 'override' in content[start:end]

    def test_header_regions_multi_line(self):
        content = """QSize
    multiLineSize(int type,
                  const QStyleOption *option,
                  const QSize &size,
                  const QWidget *widget) const override;"""
        regions = opt._find_override_regions_in_header(content)
        assert len(regions) == 1
        start, end = regions[0]
        region_text = content[start:end]
        assert 'multiLineSize' in region_text
        assert 'override' in region_text

    def test_header_regions_non_override_ignored(self):
        content = 'QSize nonOverrideSize(const QSize &size);'
        regions = opt._find_override_regions_in_header(content)
        assert len(regions) == 0


def run_all_tests():
    print("=" * 60)
    print("TDD Tests for optimize_qt_params.py - Override Skip Feature")
    print("=" * 60)

    tests = []
    passed = 0
    failed = 0

    def run(cls):
        nonlocal passed, failed
        instance = cls()
        for name in dir(instance):
            if name.startswith('test_'):
                test_fn = getattr(instance, name)
                full_name = f"{cls.__name__}.{name}"
                try:
                    test_fn()
                    passed += 1
                    print(f"  PASS: {full_name}")
                except Exception as e:
                    failed += 1
                    print(f"  FAIL: {full_name}")
                    print(f"    Error: {e}")

    run(TestOverrideSignatureExtraction)
    run(TestFindOverrideRegions)
    run(TestCollectOverrideSignatures)

    integration = TestSkipOverrideIntegration()
    integration.setup_class()
    try:
        for name in dir(integration):
            if name.startswith('test_'):
                test_fn = getattr(integration, name)
                full_name = f"TestSkipOverrideIntegration.{name}"
                try:
                    test_fn()
                    passed += 1
                    print(f"  PASS: {full_name}")
                except Exception as e:
                    failed += 1
                    print(f"  FAIL: {full_name}")
                    print(f"    Error: {e}")
    finally:
        integration.teardown_class()

    print()
    print("=" * 60)
    print(f"Results: {passed} passed, {failed} failed, {passed + failed} total")
    print("=" * 60)

    return failed == 0


if __name__ == '__main__':
    success = run_all_tests()
    sys.exit(0 if success else 1)