#!/usr/bin/env python3
"""
plan_refactoring.py - Analyze component and create refactoring plan

Analyzes a component's functions, tests, and complexity to create
a structured refactoring plan with phases, time estimates, and risk assessment.

Usage:
    python3 plan_refactoring.py --component "BrakeMonitor" \
        --input-file src/brake_monitor.cpp \
        --test-file tests/unit/brake_monitor_test.cpp \
        --output refactoring_plan.json
"""

import json
import re
import sys
from pathlib import Path
from dataclasses import dataclass, asdict
from typing import List, Optional

@dataclass
class FunctionMetrics:
    name: str
    lines_of_code: int
    complexity: int
    covering_tests: List[str]
    
@dataclass
class RefactoringPhase:
    phase_number: int
    function_name: str
    estimated_minutes: int
    covering_tests: List[str]
    current_loc: int
    risk_level: str

@dataclass
class RefactoringPlan:
    component: str
    total_functions: int
    total_loc: int
    total_covering_tests: int
    phases: List[RefactoringPhase]
    total_estimated_minutes: int
    overall_risk: str

def extract_functions(source_file: Path) -> List[tuple]:
    """Extract function names and their line counts from source file."""
    content = source_file.read_text()
    functions = []
    
    # Find function definitions (C++)
    pattern = r'(\w+)::\w+\([^)]*\)\s*(?:const)?\s*\{'
    for match in re.finditer(pattern, content):
        func_full = match.group(0)
        func_name = func_full.split('::')[1].split('(')[0]
        
        # Rough LOC estimate
        start = match.start()
        brace_count = 1
        pos = match.end()
        while brace_count > 0 and pos < len(content):
            if content[pos] == '{':
                brace_count += 1
            elif content[pos] == '}':
                brace_count -= 1
            pos += 1
        
        loc_estimate = content[start:pos].count('\n')
        functions.append((func_name, loc_estimate))
    
    return functions

def extract_covering_tests(test_file: Path, component: str) -> dict:
    """Extract test cases that cover each function."""
    content = test_file.read_text()
    test_mapping = {}
    
    # Find test cases (GoogleTest format: TC_XXX_NNN)
    pattern = r'TEST\([^,]+,\s*(\w+)\)'
    for match in re.finditer(pattern, content):
        test_name = match.group(1)
        # Extract component code (BRK, WHL, FLT, DAT)
        if 'BRK' in test_name:
            key = 'brake'
        elif 'WHL' in test_name:
            key = 'wheel'
        elif 'FLT' in test_name:
            key = 'fault'
        else:
            continue
        
        if key not in test_mapping:
            test_mapping[key] = []
        test_mapping[key].append(test_name)
    
    return test_mapping

def calculate_complexity(func_code: str) -> int:
    """Rough cyclomatic complexity estimation."""
    keywords = ['if', 'else', 'for', 'while', 'switch', 'case', '&&', '||', '?']
    complexity = 1
    for keyword in keywords:
        complexity += func_code.count(keyword)
    return max(1, complexity)

def estimate_refactoring_time(loc: int, complexity: int) -> int:
    """Estimate refactoring time in minutes based on LOC and complexity."""
    # Base: 1 min per LOC
    # Modifier: +1 min per complexity point
    time = loc + (complexity * 2)
    # Clamp to reasonable range
    return max(15, min(120, time))

def create_plan(component: str, source_file: Path, test_file: Path) -> RefactoringPlan:
    """Create comprehensive refactoring plan."""
    
    functions = extract_functions(source_file)
    test_mapping = extract_covering_tests(test_file, component)
    
    phases = []
    total_minutes = 0
    
    for i, (func_name, loc) in enumerate(functions, 1):
        # Estimate complexity (would use actual analysis in production)
        complexity = calculate_complexity(source_file.read_text())
        
        # Estimate time
        est_time = estimate_refactoring_time(loc, complexity)
        total_minutes += est_time
        
        # Determine risk
        if loc <= 10:
            risk = "VERY_LOW"
        elif loc <= 30:
            risk = "LOW"
        elif loc <= 60:
            risk = "MEDIUM"
        else:
            risk = "MEDIUM_HIGH"
        
        # Get covering tests (simplified)
        covering_tests = test_mapping.get('brake', [])[:5]
        
        phase = RefactoringPhase(
            phase_number=i,
            function_name=func_name,
            estimated_minutes=est_time,
            covering_tests=covering_tests,
            current_loc=loc,
            risk_level=risk
        )
        phases.append(phase)
    
    # Determine overall risk
    max_risk_level = max([p.risk_level for p in phases])
    if max_risk_level == "VERY_LOW":
        overall_risk = "LOW"
    elif max_risk_level in ["LOW", "MEDIUM"]:
        overall_risk = "MEDIUM"
    else:
        overall_risk = "MEDIUM_HIGH"
    
    plan = RefactoringPlan(
        component=component,
        total_functions=len(functions),
        total_loc=sum(loc for _, loc in functions),
        total_covering_tests=len([t for tests in test_mapping.values() for t in tests]),
        phases=phases,
        total_estimated_minutes=total_minutes,
        overall_risk=overall_risk
    )
    
    return plan

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Plan refactoring for a component')
    parser.add_argument('--component', required=True, help='Component name')
    parser.add_argument('--input-file', required=True, help='Source file path')
    parser.add_argument('--test-file', required=True, help='Test file path')
    parser.add_argument('--output', default='refactoring_plan.json', help='Output file')
    
    args = parser.parse_args()
    
    source_file = Path(args.input_file)
    test_file = Path(args.test_file)
    
    if not source_file.exists():
        print(f"Error: Source file not found: {source_file}")
        sys.exit(1)
    
    if not test_file.exists():
        print(f"Error: Test file not found: {test_file}")
        sys.exit(1)
    
    plan = create_plan(args.component, source_file, test_file)
    
    # Convert to JSON
    plan_dict = asdict(plan)
    plan_dict['phases'] = [asdict(p) for p in plan.phases]
    
    output_file = Path(args.output)
    with open(output_file, 'w') as f:
        json.dump(plan_dict, f, indent=2)
    
    print(f"✓ Refactoring plan created: {output_file}")
    print(f"  Component: {plan.component}")
    print(f"  Functions: {plan.total_functions}")
    print(f"  Total LOC: {plan.total_loc}")
    print(f"  Phases: {len(plan.phases)}")
    print(f"  Estimated Time: {plan.total_estimated_minutes} minutes ({plan.total_estimated_minutes/60:.1f} hours)")
    print(f"  Risk Level: {plan.overall_risk}")

if __name__ == '__main__':
    main()
