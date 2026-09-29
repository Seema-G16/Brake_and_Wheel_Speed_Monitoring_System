#!/usr/bin/env python3
"""
eval_unit_size.py - Verify unit size is appropriate.

Checks:
- Function LOC: 3-30 (not too small, not too large)
- Covering tests: 3-5 (adequate coverage)
- Estimated time: 15-90 min (fits workflow)
- Risk level: LOW
"""

import os
import sys
import argparse
import json
from pathlib import Path

def count_lines_of_code(source_file, function_name):
    """Count LOC for a specific function."""
    if not os.path.exists(source_file):
        print(f"❌ File not found: {source_file}")
        return 0
    
    with open(source_file, 'r') as f:
        lines = f.readlines()
    
    in_function = False
    loc = 0
    brace_count = 0
    
    for line in lines:
        if function_name in line and '{' in line:
            in_function = True
            brace_count = line.count('{') - line.count('}')
            loc = 1
        elif in_function:
            brace_count += line.count('{') - line.count('}')
            if brace_count <= 0:
                break
            if line.strip() and not line.strip().startswith('//'):
                loc += 1
    
    return loc

def evaluate_unit_size(unit_id, function_name, loc, num_tests, est_time_minutes):
    """Evaluate if unit size is appropriate."""
    print(f"\nUnit {unit_id}: {function_name}")
    print("-" * 40)
    
    checks = []
    
    # Check LOC
    loc_ok = 3 <= loc <= 30
    print(f"Lines of Code: {loc} {'✓' if loc_ok else '❌ (3-30 expected)'}")
    checks.append(loc_ok)
    
    # Check tests
    tests_ok = 3 <= num_tests <= 5
    print(f"Covering Tests: {num_tests} {'✓' if tests_ok else '❌ (3-5 expected)'}")
    checks.append(tests_ok)
    
    # Check time
    time_ok = 15 <= est_time_minutes <= 90
    print(f"Est. Time: {est_time_minutes} min {'✓' if time_ok else '❌ (15-90 expected)'}")
    checks.append(time_ok)
    
    # Determine risk
    if all(checks):
        risk = "LOW"
        print(f"Risk Level: {risk} ✓")
    elif loc > 30 or est_time_minutes > 90:
        risk = "MEDIUM"
        print(f"Risk Level: {risk} ⚠")
    else:
        risk = "HIGH"
        print(f"Risk Level: {risk} ❌")
    
    return all(checks)

def main():
    parser = argparse.ArgumentParser(
        description='Verify unit size is appropriate'
    )
    parser.add_argument('--plan-file', default='refactoring_plan.json',
                       help='Refactoring plan JSON file')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.plan_file):
        print(f"❌ Plan file not found: {args.plan_file}")
        print("Run: python3 scripts/plan_refactoring.py first")
        return 1
    
    with open(args.plan_file, 'r') as f:
        plan = json.load(f)
    
    print(f"📐 Evaluating unit sizes for: {plan.get('component', 'Unknown')}")
    print("=" * 60)
    
    results = []
    for unit in plan.get('functions', []):
        result = evaluate_unit_size(
            unit.get('id', 0),
            unit.get('name', 'unknown'),
            unit.get('loc', 0),
            unit.get('covering_tests', 3),
            unit.get('estimated_refactoring_time_minutes', 45)
        )
        results.append(result)
    
    print("\n" + "=" * 60)
    if all(results):
        print("✓ ALL UNITS ARE APPROPRIATELY SIZED!")
        return 0
    else:
        print("❌ SOME UNITS NEED ADJUSTMENT")
        print("Consider breaking large units into smaller ones.")
        return 1

if __name__ == '__main__':
    sys.exit(main())
