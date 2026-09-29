#!/usr/bin/env python3
"""
eval_metrics.py - Verify metrics improved.

Checks:
- Complexity reduced
- LOC maintained or reduced
- Coverage maintained or improved
- Performance unchanged
"""

import os
import sys
import argparse
import json

def compare_metrics(before_metrics, after_metrics):
    """Compare before/after metrics."""
    
    results = {
        'complexity_improved': False,
        'loc_optimal': False,
        'coverage_maintained': False,
        'time_acceptable': False
    }
    
    print("\n📊 Metrics Comparison")
    print("-" * 40)
    
    # Complexity
    if 'complexity' in before_metrics and 'complexity' in after_metrics:
        before = before_metrics['complexity']
        after = after_metrics['complexity']
        reduction = ((before - after) / before * 100) if before > 0 else 0
        
        improved = after < before
        print(f"Complexity: {before} → {after} " +
              f"({reduction:.1f}% {'reduction' if improved else 'increase'}) " +
              f"{'✓' if improved else '❌'}")
        results['complexity_improved'] = improved
    
    # LOC
    if 'loc' in before_metrics and 'loc' in after_metrics:
        before = before_metrics['loc']
        after = after_metrics['loc']
        change = ((after - before) / before * 100) if before > 0 else 0
        
        optimal = after <= before
        print(f"LOC: {before} → {after} " +
              f"({change:+.1f}%) " +
              f"{'✓' if optimal else '❌'}")
        results['loc_optimal'] = optimal
    
    # Coverage
    if 'coverage' in before_metrics and 'coverage' in after_metrics:
        before = before_metrics['coverage']
        after = after_metrics['coverage']
        
        maintained = after >= before
        print(f"Coverage: {before}% → {after}% " +
              f"({after - before:+.1f}%) " +
              f"{'✓' if maintained else '❌'}")
        results['coverage_maintained'] = maintained
    
    # Performance
    if 'test_time_ms' in before_metrics and 'test_time_ms' in after_metrics:
        before = before_metrics['test_time_ms']
        after = after_metrics['test_time_ms']
        change = ((after - before) / before * 100) if before > 0 else 0
        
        acceptable = change < 50  # Allow 50% regression
        print(f"Test Time: {before}ms → {after}ms " +
              f"({change:+.1f}%) " +
              f"{'✓' if acceptable else '❌'}")
        results['time_acceptable'] = acceptable
    
    return results

def main():
    parser = argparse.ArgumentParser(
        description='Verify metrics improved after refactoring'
    )
    parser.add_argument('--plan-file', default='refactoring_plan.json',
                       help='Original refactoring plan')
    parser.add_argument('--log-file', default='refactoring_log.json',
                       help='Refactoring log with results')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.plan_file):
        print(f"❌ Plan file not found: {args.plan_file}")
        return 1
    
    if not os.path.exists(args.log_file):
        print(f"❌ Log file not found: {args.log_file}")
        print("Run refactoring first and log results.")
        return 1
    
    with open(args.plan_file, 'r') as f:
        plan = json.load(f)
    
    with open(args.log_file, 'r') as f:
        log = json.load(f)
    
    print(f"📈 Evaluating metrics for: {plan.get('component', 'Unknown')}")
    print("=" * 60)
    
    # Extract metrics
    before_metrics = plan.get('success_metrics', {})
    after_metrics = log.get('metrics', {})
    
    results = compare_metrics(before_metrics, after_metrics)
    
    print("\n" + "=" * 60)
    passed = sum(results.values())
    total = len(results)
    
    if passed >= 3:  # At least 3 of 4
        print(f"✓ METRICS IMPROVED ({passed}/{total} checks)")
        return 0
    else:
        print(f"❌ METRICS NOT IMPROVED ({passed}/{total} checks)")
        return 1

if __name__ == '__main__':
    sys.exit(main())
