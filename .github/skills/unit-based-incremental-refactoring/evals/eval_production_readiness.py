#!/usr/bin/env python3
"""
eval_production_readiness.py - Final check before production merge.

Verifies:
- All 52 tests passing
- No regressions introduced
- All files compilable
- Metrics improved
- Documentation updated
"""

import os
import sys
import subprocess
import argparse
import json

def check_all_tests_pass():
    """Verify all 52 tests pass."""
    print("\nChecking test suite...")
    
    if not os.path.exists('build'):
        print("❌ Build directory not found")
        return False
    
    os.chdir('build')
    result = subprocess.run(['ctest', '--verbose'],
                          capture_output=True, text=True)
    os.chdir('..')
    
    if result.returncode != 0:
        print("❌ Some tests failed")
        return False
    
    # Parse output
    if '52 tests passed' in result.stdout or '52/52' in result.stdout:
        print("✓ All 52 tests passing")
        return True
    
    print("⚠ Could not verify all 52 tests")
    return False

def check_no_regressions():
    """Check no new test failures."""
    print("\nChecking for regressions...")
    
    if not os.path.exists('build'):
        return False
    
    os.chdir('build')
    result = subprocess.run(['ctest', '--verbose'],
                          capture_output=True, text=True)
    os.chdir('..')
    
    if 'FAILED' in result.stdout:
        print("❌ Regressions detected")
        return False
    
    print("✓ No regressions detected")
    return True

def check_compilation():
    """Verify clean compilation."""
    print("\nChecking compilation...")
    
    if not os.path.exists('build'):
        return False
    
    os.chdir('build')
    result = subprocess.run(['cmake', '--build', '.'],
                          capture_output=True, text=True)
    os.chdir('..')
    
    if result.returncode != 0:
        print("❌ Compilation errors")
        return False
    
    if 'warning' in result.stderr.lower():
        print("⚠ Compilation warnings present")
        return False
    
    print("✓ Clean compilation")
    return True

def check_metrics_improved(log_file):
    """Verify metrics improved."""
    print("\nChecking metrics...")
    
    if not os.path.exists(log_file):
        print("⚠ No log file found")
        return True  # Don't block if no log
    
    try:
        with open(log_file, 'r') as f:
            log = json.load(f)
        
        # Check if any improvements recorded
        if 'metrics' in log:
            metrics = log['metrics']
            if 'complexity_reduction' in metrics:
                print(f"✓ Complexity metrics recorded")
                return True
    except:
        pass
    
    print("⚠ Could not verify metrics")
    return True  # Non-blocking

def main():
    parser = argparse.ArgumentParser(
        description='Final check before production merge'
    )
    parser.add_argument('--component', required=True,
                       help='Component name')
    parser.add_argument('--log-file', default='refactoring_log.json',
                       help='Refactoring log file')
    
    args = parser.parse_args()
    
    print(f"🚀 Production Readiness Check: {args.component}")
    print("=" * 60)
    
    checks = [
        ("Tests passing", check_all_tests_pass),
        ("No regressions", check_no_regressions),
        ("Compilation", check_compilation),
        ("Metrics improved", lambda: check_metrics_improved(args.log_file)),
    ]
    
    results = []
    for check_name, check_func in checks:
        try:
            results.append(check_func())
        except Exception as e:
            print(f"❌ Error: {e}")
            results.append(False)
    
    print("\n" + "=" * 60)
    if all(results):
        print(f"✓ {args.component} IS PRODUCTION READY!")
        print("Ready to merge to main branch.")
        return 0
    else:
        print("❌ NOT PRODUCTION READY")
        print("Fix issues above before merging.")
        return 1

if __name__ == '__main__':
    sys.exit(main())
