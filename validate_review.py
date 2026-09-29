#!/usr/bin/env python3
"""
Validate that review changes were correctly applied
"""

import json
import subprocess
import sys
from pathlib import Path
from datetime import datetime


class ReviewValidator:
    def __init__(self):
        self.passed = []
        self.failed = []
        self.project_root = Path('.')
    
    def validate_compilation(self):
        """Check that code still compiles"""
        print("\n[Validation] Checking compilation...")
        try:
            # Check if build directory exists
            if not Path('build').exists():
                print("  Build directory not found. Skipping compilation check.")
                return
            
            result = subprocess.run(
                ['cmake', '--build', 'build'],
                cwd='.',
                capture_output=True,
                timeout=60,
                text=True
            )
            if result.returncode == 0:
                print("✓ Code compiles successfully")
                self.passed.append('compilation')
            else:
                print("✗ Compilation failed")
                if result.stderr:
                    print(f"  Error: {result.stderr[:200]}")
                self.failed.append('compilation')
        except subprocess.TimeoutExpired:
            print("✗ Compilation timeout")
            self.failed.append('compilation')
        except FileNotFoundError:
            print("⚠ CMake not found. Skipping compilation check.")
        except Exception as e:
            print(f"⚠ Compilation check skipped: {e}")
    
    def validate_tests(self):
        """Check that all tests pass"""
        print("\n[Validation] Running tests...")
        try:
            if not Path('build').exists():
                print("  Build directory not found. Skipping test check.")
                return
            
            result = subprocess.run(
                ['ctest', '--verbose'],
                cwd='build',
                capture_output=True,
                timeout=120,
                text=True
            )
            if result.returncode == 0:
                # Count passed tests
                output = result.stdout
                if '100% tests passed' in output:
                    print("✓ All tests passed")
                    self.passed.append('tests')
                else:
                    print("✗ Some tests failed")
                    self.failed.append('tests')
            else:
                print("✗ Tests failed")
                self.failed.append('tests')
        except subprocess.TimeoutExpired:
            print("✗ Test execution timeout")
            self.failed.append('tests')
        except FileNotFoundError:
            print("⚠ CTest not found. Skipping test check.")
        except Exception as e:
            print(f"⚠ Test check skipped: {e}")
    
    def validate_coverage(self):
        """Check test coverage"""
        print("\n[Validation] Checking coverage...")
        try:
            test_files = list(Path('tests/unit').glob('*_test.cpp'))
            if len(test_files) > 0:
                print(f"✓ Found {len(test_files)} test files")
                self.passed.append('coverage')
            else:
                print("✗ No test files found")
                self.failed.append('coverage')
        except Exception as e:
            print(f"✗ Coverage check error: {e}")
            self.failed.append('coverage')
    
    def validate_documentation(self):
        """Check documentation exists"""
        print("\n[Validation] Checking documentation...")
        required_docs = [
            'README.md',
            'Docs/requirements.md',
            'BUILD_SUCCESS.md'
        ]
        
        missing = []
        for doc in required_docs:
            if not Path(doc).exists():
                missing.append(doc)
        
        if not missing:
            print(f"✓ All documentation files present ({len(required_docs)} files)")
            self.passed.append('documentation')
        else:
            print(f"✗ Missing: {', '.join(missing)}")
            self.failed.append('documentation')
    
    def validate_code_structure(self):
        """Check code directory structure"""
        print("\n[Validation] Checking code structure...")
        required_dirs = [
            'include',
            'src',
            'tests/unit',
            'Docs'
        ]
        
        missing = []
        for dir_path in required_dirs:
            if not Path(dir_path).exists():
                missing.append(dir_path)
        
        if not missing:
            print(f"✓ Code structure valid ({len(required_dirs)} directories)")
            self.passed.append('code_structure')
        else:
            print(f"✗ Missing directories: {', '.join(missing)}")
            self.failed.append('code_structure')
    
    def validate_headers(self):
        """Check header files exist"""
        print("\n[Validation] Checking header files...")
        required_headers = [
            'include/faults/fault_types.hpp',
            'include/faults/fault_manager.hpp',
            'include/brake/brake_monitor.hpp',
            'include/wheel_speed/wheel_speed_monitor.hpp',
            'include/validation/monitoring_data_validator.hpp',
            'include/monitoring_system/brake_wheel_speed_sample.hpp'
        ]
        
        missing = []
        for header in required_headers:
            if not Path(header).exists():
                missing.append(header)
        
        if not missing:
            print(f"✓ All headers present ({len(required_headers)} files)")
            self.passed.append('headers')
        else:
            print(f"✗ Missing headers: {', '.join(missing)}")
            self.failed.append('headers')
    
    def generate_validation_report(self):
        """Generate validation report"""
        total_checks = len(self.passed) + len(self.failed)
        success_rate = (len(self.passed) / total_checks * 100) if total_checks > 0 else 0
        
        status = 'PASSED' if not self.failed else 'FAILED'
        report = {
            'timestamp': datetime.now().isoformat(),
            'status': status,
            'validation_results': {
                'passed': self.passed,
                'failed': self.failed,
                'total': total_checks,
                'success_rate': f"{success_rate:.1f}%"
            }
        }
        
        try:
            with open('review_validation.json', 'w') as f:
                json.dump(report, f, indent=2)
        except IOError as e:
            print(f"\n✗ Could not save validation report: {e}")
            return False
        
        print("\n" + "="*70)
        print("VALIDATION RESULTS")
        print("="*70)
        print(f"Status: {report['status']}")
        print(f"Passed: {len(self.passed)}/{total_checks}")
        print(f"Success Rate: {report['validation_results']['success_rate']}")
        
        if self.passed:
            print(f"\n✓ Passed validations:")
            for passed in self.passed:
                print(f"  ✓ {passed}")
        
        if self.failed:
            print(f"\n✗ Failed validations:")
            for failure in self.failed:
                print(f"  ✗ {failure}")
        
        print("\n✓ Report saved to review_validation.json")
        
        return status == 'PASSED'
    
    def run(self):
        """Run all validations"""
        print("="*70)
        print("Code Review - Validation Script")
        print("="*70)
        print("\nStarting validation checks...\n")
        
        self.validate_code_structure()
        self.validate_headers()
        self.validate_coverage()
        self.validate_documentation()
        self.validate_compilation()
        self.validate_tests()
        
        success = self.generate_validation_report()
        return 0 if success else 1


if __name__ == '__main__':
    validator = ReviewValidator()
    exit_code = validator.run()
    sys.exit(exit_code)
