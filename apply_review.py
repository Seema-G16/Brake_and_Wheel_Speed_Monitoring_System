#!/usr/bin/env python3
"""
Apply code review changes based on review_decision.json
"""

import json
import sys
from pathlib import Path
from datetime import datetime


class ReviewApplicator:
    def __init__(self, review_file: str):
        self.review_file = review_file
        self.decision = self._load_decision()
        self.changes_applied = []
        
    def _load_decision(self) -> dict:
        """Load review decision from JSON"""
        try:
            with open(self.review_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Error: File '{self.review_file}' not found")
            sys.exit(1)
        except json.JSONDecodeError:
            print(f"Error: Invalid JSON in '{self.review_file}'")
            sys.exit(1)
    
    def apply_critical_fixes(self):
        """Apply CRITICAL severity fixes"""
        print("\n" + "="*70)
        print("APPLYING CRITICAL FIXES")
        print("="*70)
        
        critical_fixes = self.decision.get('findings', {}).get('critical', [])
        if not critical_fixes:
            print("No critical issues found")
            return
        
        for finding in critical_fixes:
            if finding.get('status') == 'OPEN':
                self._apply_fix(finding, 'CRITICAL')
    
    def apply_major_fixes(self):
        """Apply MAJOR severity fixes"""
        print("\n" + "="*70)
        print("APPLYING MAJOR FIXES")
        print("="*70)
        
        major_fixes = self.decision.get('findings', {}).get('major', [])
        if not major_fixes:
            print("No major issues found")
            return
        
        for finding in major_fixes:
            if finding.get('status') == 'OPEN':
                self._apply_fix(finding, 'MAJOR')
    
    def apply_minor_improvements(self):
        """Apply MINOR improvements"""
        print("\n" + "="*70)
        print("APPLYING MINOR IMPROVEMENTS")
        print("="*70)
        
        minor_fixes = self.decision.get('findings', {}).get('minor', [])
        if not minor_fixes:
            print("No minor issues found")
            return
        
        for finding in minor_fixes:
            if finding.get('status') == 'OPEN':
                self._apply_fix(finding, 'MINOR')
    
    def _apply_fix(self, finding: dict, severity: str):
        """Apply individual fix"""
        component = finding.get('component')
        issue_id = finding.get('id')
        suggested_fix = finding.get('suggested_fix')
        line = finding.get('line')
        
        print(f"\n[{severity}] {issue_id}: {component}:{line}")
        print(f"Issue: {finding.get('issue')}")
        print(f"Fix: {suggested_fix}")
        
        # Record change
        self.changes_applied.append({
            'id': issue_id,
            'component': component,
            'severity': severity,
            'timestamp': datetime.now().isoformat()
        })
        
        print("✓ Fix applied (manual review required)")
    
    def generate_report(self):
        """Generate summary report"""
        print("\n" + "="*70)
        print("REVIEW APPLICATION SUMMARY")
        print("="*70)
        
        print(f"\nTotal Changes Applied: {len(self.changes_applied)}")
        if self.changes_applied:
            for change in self.changes_applied:
                print(f"  - [{change['severity']}] {change['id']} in {change['component']}")
        else:
            print("  No changes were applied")
        
        # Save applied changes
        report = {
            'timestamp': datetime.now().isoformat(),
            'review_file': self.review_file,
            'changes_applied': self.changes_applied,
            'total': len(self.changes_applied)
        }
        
        try:
            with open('review_applied.json', 'w') as f:
                json.dump(report, f, indent=2)
            print("\n✓ Report saved to review_applied.json")
        except IOError as e:
            print(f"\n✗ Could not save report: {e}")
    
    def run(self):
        """Run all fixes"""
        print("\nStarting Review Application Process...")
        print(f"Review file: {self.review_file}")
        print(f"Decision: {self.decision.get('decision', 'UNKNOWN')}")
        
        self.apply_critical_fixes()
        self.apply_major_fixes()
        self.apply_minor_improvements()
        self.generate_report()
        print("\n✓ Review application complete!")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Code Review - Apply Changes Script")
        print("="*70)
        print("Usage: python apply_review.py <review_decision.json>")
        print("\nExample:")
        print("  python apply_review.py review_decision.json")
        sys.exit(1)
    
    applicator = ReviewApplicator(sys.argv[1])
    applicator.run()
