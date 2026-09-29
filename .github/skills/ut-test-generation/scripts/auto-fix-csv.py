#!/usr/bin/env python3
"""
CSV Auto-Fix Script for Brake & Wheel-Speed Monitoring System UT Test Spec
Automatically fixes common validation issues in the testspec.csv.
"""

import csv
import sys
import re
from pathlib import Path
from typing import List, Tuple

# Valid test case types
VALID_TEST_CASE_TYPES = ["Positive", "Negative", "Edge", "Recovery", "Boundary"]

# Automated values
VALID_AUTOMATED = ["Yes", "No"]

# Test case ID pattern: TC-XXX-NNN
TESTCASE_ID_PATTERN = re.compile(r"^TC-(BRK|WHL|FLT|DAT)-\d{3}$")
MALFORMED_ID_PATTERN = re.compile(r"^TC-(BRK|WHL|FLT|DAT)-(\d{1,3})$")

class CSVAutoFixer:
    def __init__(self, csv_path: str, max_attempts: int = 3):
        self.csv_path = Path(csv_path)
        self.max_attempts = max_attempts
        self.fixes_applied: List[str] = []
        self.unfixable_errors: List[str] = []
    
    def fix(self) -> Tuple[bool, List[str], List[str]]:
        """
        Attempt to auto-fix the CSV file.
        Returns (success, fixes_applied, unfixable_errors)
        """
        
        if not self.csv_path.exists():
            self.unfixable_errors.append(f"CSV file not found: {self.csv_path}")
            return False, self.fixes_applied, self.unfixable_errors
        
        attempt = 0
        while attempt < self.max_attempts:
            attempt += 1
            try:
                rows = self._read_csv()
                if rows is None:
                    return False, self.fixes_applied, self.unfixable_errors
                
                fixed_rows = self._fix_rows(rows)
                self._write_csv(fixed_rows)
                
                print(f"✓ Fix attempt {attempt} completed.", file=sys.stderr)
                
                # If no fixes were needed this iteration, we're done
                if not self.fixes_applied or attempt >= self.max_attempts:
                    break
                    
            except Exception as e:
                self.unfixable_errors.append(f"Error during fix attempt {attempt}: {e}")
                return False, self.fixes_applied, self.unfixable_errors
        
        return len(self.unfixable_errors) == 0, self.fixes_applied, self.unfixable_errors
    
    def _read_csv(self) -> List[dict]:
        """Read CSV file."""
        try:
            rows = []
            with open(self.csv_path, 'r', encoding='utf-8') as f:
                reader = csv.DictReader(f)
                if reader.fieldnames is None:
                    self.unfixable_errors.append("CSV file is empty or malformed.")
                    return None
                for row in reader:
                    rows.append(row)
            return rows
        except Exception as e:
            self.unfixable_errors.append(f"Error reading CSV: {e}")
            return None
    
    def _fix_rows(self, rows: List[dict]) -> List[dict]:
        """Fix individual rows."""
        fixed_rows = []
        for row_num, row in enumerate(rows, start=2):
            fixed_row = self._fix_row(row, row_num)
            fixed_rows.append(fixed_row)
        return fixed_rows
    
    def _fix_row(self, row: dict, row_num: int) -> dict:
        """Fix a single row."""
        
        # Fix Testcase ID format (pad with zeros if needed)
        testcase_id = row.get("Testcase ID", "").strip()
        if testcase_id:
            if TESTCASE_ID_PATTERN.match(testcase_id):
                pass  # Already valid
            else:
                match = MALFORMED_ID_PATTERN.match(testcase_id)
                if match:
                    prefix, num = match.groups()
                    fixed_id = f"TC-{prefix}-{num.zfill(3)}"
                    row["Testcase ID"] = fixed_id
                    self.fixes_applied.append(f"Row {row_num}: Fixed Testcase ID from '{testcase_id}' to '{fixed_id}'")
                else:
                    self.unfixable_errors.append(f"Row {row_num}: Cannot auto-fix Testcase ID '{testcase_id}'")
        
        # Ensure Testcase Name is not empty (use ID as fallback)
        if not row.get("Testcase Name", "").strip():
            fallback = row.get("Testcase ID", "TC-UNKNOWN").replace("TC-", "").replace("-", " ")
            row["Testcase Name"] = f"Test case {fallback}"
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Testcase Name")
        
        # Ensure Test Description is not empty
        if not row.get("Test Description", "").strip():
            row["Test Description"] = row.get("Testcase Name", "Test case")
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Test Description")
        
        # Ensure Input is not empty (use generic placeholder)
        if not row.get("Input", "").strip():
            row["Input"] = "Standard test input values"
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Input")
        
        # Fix Test Case Type
        test_type = row.get("Test Case Type", "").strip()
        if not test_type:
            row["Test Case Type"] = "Positive"  # Default
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Test Case Type as 'Positive'")
        elif test_type not in VALID_TEST_CASE_TYPES:
            # Try to infer from description
            desc = row.get("Test Description", "").lower()
            if "boundary" in desc or "edge" in desc:
                row["Test Case Type"] = "Boundary"
                self.fixes_applied.append(f"Row {row_num}: Corrected Test Case Type to 'Boundary'")
            elif "fault" in desc or "error" in desc or "invalid" in desc:
                row["Test Case Type"] = "Negative"
                self.fixes_applied.append(f"Row {row_num}: Corrected Test Case Type to 'Negative'")
            elif "recovery" in desc:
                row["Test Case Type"] = "Recovery"
                self.fixes_applied.append(f"Row {row_num}: Corrected Test Case Type to 'Recovery'")
            else:
                row["Test Case Type"] = "Positive"
                self.fixes_applied.append(f"Row {row_num}: Corrected Test Case Type to 'Positive'")
        
        # Ensure Expected Output is not empty
        if not row.get("Expected Output", "").strip():
            row["Expected Output"] = "Test passes with no faults"
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Expected Output")
        
        # Fix Automated (Yes/No)
        automated = row.get("Automated (Yes/No)", "").strip()
        if not automated:
            row["Automated (Yes/No)"] = "Yes"
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Automated as 'Yes'")
        elif automated.lower() in ["true", "y", "1", "yes"]:
            row["Automated (Yes/No)"] = "Yes"
            if automated != "Yes":
                self.fixes_applied.append(f"Row {row_num}: Normalized Automated to 'Yes'")
        elif automated.lower() in ["false", "n", "0", "no"]:
            row["Automated (Yes/No)"] = "No"
            if automated != "No":
                self.fixes_applied.append(f"Row {row_num}: Normalized Automated to 'No'")
        
        # Ensure Actual Output is set to "Pending" if empty
        if not row.get("Actual Output", "").strip():
            row["Actual Output"] = "Pending"
            self.fixes_applied.append(f"Row {row_num}: Auto-populated Actual Output as 'Pending'")
        
        return row
    
    def _write_csv(self, rows: List[dict]):
        """Write fixed rows back to CSV."""
        fieldnames = [
            "Testcase ID",
            "Testcase Name",
            "Test Description",
            "Input",
            "Test Case Type",
            "Expected Output",
            "Actual Output",
            "Automated (Yes/No)",
            "Comments"
        ]
        
        with open(self.csv_path, 'w', encoding='utf-8', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(rows)


def main():
    if len(sys.argv) < 2:
        print("Usage: python auto-fix-csv.py <path-to-testspec.csv> [max-attempts]", file=sys.stderr)
        sys.exit(1)
    
    csv_path = sys.argv[1]
    max_attempts = int(sys.argv[2]) if len(sys.argv) > 2 else 3
    
    fixer = CSVAutoFixer(csv_path, max_attempts=max_attempts)
    success, fixes, errors = fixer.fix()
    
    if fixes:
        print(f"\n🔧 Applied {len(fixes)} fixes:", file=sys.stderr)
        for fix in fixes:
            print(f"  • {fix}", file=sys.stderr)
    
    if errors:
        print(f"\n⚠️  {len(errors)} unfixable errors:", file=sys.stderr)
        for error in errors:
            print(f"  • {error}", file=sys.stderr)
    
    if success:
        print("\n✅ Auto-fix completed successfully!", file=sys.stderr)
        sys.exit(0)
    else:
        print("\n❌ Auto-fix completed with errors. Manual intervention may be needed.", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
