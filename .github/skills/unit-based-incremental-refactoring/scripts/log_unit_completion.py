#!/usr/bin/env python3
"""
log_unit_completion.py - Record unit completion with metrics

Records completion of each refactored unit including test results,
metrics, time spent, and status for later reporting.

Usage:
    python3 log_unit_completion.py \
        --unit-id 1 \
        --function-name "is_braking" \
        --status "PASS" \
        --time-minutes 15 \
        --complexity-before 2 \
        --complexity-after 1 \
        --tests-passed 3 \
        --tests-total 3
"""

import json
import sys
from pathlib import Path
from datetime import datetime
from dataclasses import dataclass, asdict

@dataclass
class UnitCompletion:
    unit_id: int
    function_name: str
    timestamp: str
    status: str  # PASS, FAIL, INCOMPLETE
    time_minutes: int
    covering_tests_passed: int
    covering_tests_total: int
    full_suite_passed: int
    full_suite_total: int
    complexity_before: int
    complexity_after: int
    loc_before: int
    loc_after: int
    notes: str

def load_log(log_file: Path) -> list:
    """Load existing log entries."""
    if log_file.exists():
        with open(log_file, 'r') as f:
            return json.load(f)
    return []

def save_log(log_file: Path, entries: list):
    """Save log entries."""
    with open(log_file, 'w') as f:
        json.dump(entries, f, indent=2)

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Log unit refactoring completion')
    parser.add_argument('--unit-id', type=int, required=True, help='Unit phase number')
    parser.add_argument('--function-name', required=True, help='Function name')
    parser.add_argument('--status', required=True, choices=['PASS', 'FAIL', 'INCOMPLETE'])
    parser.add_argument('--time-minutes', type=int, required=True, help='Time spent in minutes')
    parser.add_argument('--complexity-before', type=int, required=True)
    parser.add_argument('--complexity-after', type=int, required=True)
    parser.add_argument('--loc-before', type=int, default=0)
    parser.add_argument('--loc-after', type=int, default=0)
    parser.add_argument('--tests-passed', type=int, default=0)
    parser.add_argument('--tests-total', type=int, default=0)
    parser.add_argument('--full-suite-passed', type=int, default=0)
    parser.add_argument('--full-suite-total', type=int, default=0)
    parser.add_argument('--notes', default='')
    parser.add_argument('--log-file', default='refactoring_log.json')
    
    args = parser.parse_args()
    
    log_file = Path(args.log_file)
    entries = load_log(log_file)
    
    completion = UnitCompletion(
        unit_id=args.unit_id,
        function_name=args.function_name,
        timestamp=datetime.now().isoformat(),
        status=args.status,
        time_minutes=args.time_minutes,
        covering_tests_passed=args.tests_passed,
        covering_tests_total=args.tests_total,
        full_suite_passed=args.full_suite_passed,
        full_suite_total=args.full_suite_total,
        complexity_before=args.complexity_before,
        complexity_after=args.complexity_after,
        loc_before=args.loc_before,
        loc_after=args.loc_after,
        notes=args.notes
    )
    
    entries.append(asdict(completion))
    save_log(log_file, entries)
    
    # Print summary
    complexity_reduction = args.complexity_before - args.complexity_after
    complexity_pct = (complexity_reduction / args.complexity_before * 100) if args.complexity_before > 0 else 0
    
    print(f"✓ Unit {args.unit_id} logged: {args.function_name}")
    print(f"  Status: {args.status}")
    print(f"  Time: {args.time_minutes} minutes")
    print(f"  Tests: {args.tests_passed}/{args.tests_total} passed")
    print(f"  Full Suite: {args.full_suite_passed}/{args.full_suite_total} passed")
    print(f"  Complexity: {args.complexity_before} → {args.complexity_after} ({complexity_pct:.0f}% reduction)")
    print(f"  Log: {log_file}")

if __name__ == '__main__':
    main()
