#!/usr/bin/env python3
"""
CSV Validation Script for Brake & Wheel-Speed Monitoring System UT Test Spec
Validates CSV structure, format, and requirement traceability.
"""

import csv
import sys
import re
from pathlib import Path
from typing import List, Set, Tuple

# Required columns in the testspec.csv
REQUIRED_COLUMNS = [
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

# Valid test case types
VALID_TEST_CASE_TYPES = ["Positive", "Negative", "Edge", "Recovery", "Boundary"]

# Automated values
VALID_AUTOMATED = ["Yes", "No"]

# Expected requirement IDs
EXPECTED_REQUIREMENTS = {
    "BRK": {"01", "02", "03", "04", "05"},
    "WHL": {"01", "02", "03", "04", "05", "06", "07", "08"},
    "FLT": {"01", "02", "03", "04"},
    "DAT": {"01", "02", "03", "04"}
}

# Test case ID pattern: TC-XXX-NNN (e.g., TC-BRK-001)
TESTCASE_ID_PATTERN = re.compile(r"^TC-(BRK|WHL|FLT|DAT)-\d{3}$")

class CSVValidator:
    def __init__(self, csv_path: str):
        self.csv_path = Path(csv_path)
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.covered_requirements: Set[str] = set()
        
    def validate(self) -> Tuple[bool, List[str], List[str]]:
        """Validate the CSV file. Returns (is_valid, errors, warnings)."""
        
        if not self.csv_path.exists():
            self.errors.append(f"CSV file not found: {self.csv_path}")
            return False, self.errors, self.warnings
        
        try:
            with open(self.csv_path, 'r', encoding='utf-8') as f:
                reader = csv.DictReader(f)
                
                if reader.fieldnames is None:
                    self.errors.append("CSV file is empty or malformed.")
                    return False, self.errors, self.warnings
                
                # Check required columns
                self._validate_columns(reader.fieldnames)
                if self.errors:
                    return False, self.errors, self.warnings
                
                # Validate rows
                for row_num, row in enumerate(reader, start=2):  # start at 2 (header is row 1)
                    self._validate_row(row, row_num)
            
            # Check requirement coverage
            self._check_requirement_coverage()
            
        except Exception as e:
            self.errors.append(f"Error reading CSV: {e}")
            return False, self.errors, self.warnings
        
        is_valid = len(self.errors) == 0
        return is_valid, self.errors, self.warnings
    
    def _validate_columns(self, fieldnames: List[str]):
        """Validate that all required columns are present."""
        missing = set(REQUIRED_COLUMNS) - set(fieldnames)
        if missing:
            self.errors.append(f"Missing required columns: {', '.join(missing)}")
        
        extra = set(fieldnames) - set(REQUIRED_COLUMNS)
        if extra:
            self.warnings.append(f"Extra columns found (not required): {', '.join(extra)}")
    
    def _validate_row(self, row: dict, row_num: int):
        """Validate a single row in the CSV."""
        
        # Check Testcase ID format
        testcase_id = row.get("Testcase ID", "").strip()
        if not testcase_id:
            self.errors.append(f"Row {row_num}: Testcase ID is empty.")
        elif not TESTCASE_ID_PATTERN.match(testcase_id):
            self.errors.append(f"Row {row_num}: Invalid Testcase ID format '{testcase_id}'. Expected TC-XXX-NNN (e.g., TC-BRK-001).")
        else:
            # Extract requirement ID from testcase ID
            req_prefix = testcase_id.split("-")[1]  # e.g., "BRK"
            req_num = testcase_id.split("-")[2]      # e.g., "001"
            # Convert "001" to "01" (strip only trailing zeros for display, keep leading zero)
            req_num_normalized = str(int(req_num)).zfill(2) if int(req_num) <= 99 else str(int(req_num))
            self.covered_requirements.add(f"{req_prefix}-{req_num_normalized}")
        
        # Check Testcase Name
        name = row.get("Testcase Name", "").strip()
        if not name:
            self.errors.append(f"Row {row_num}: Testcase Name is empty.")
        
        # Check Test Description
        desc = row.get("Test Description", "").strip()
        if not desc:
            self.errors.append(f"Row {row_num}: Test Description is empty.")
        
        # Check Input
        input_val = row.get("Input", "").strip()
        if not input_val:
            self.errors.append(f"Row {row_num}: Input is empty.")
        
        # Check Test Case Type
        test_type = row.get("Test Case Type", "").strip()
        if not test_type:
            self.errors.append(f"Row {row_num}: Test Case Type is empty.")
        elif test_type not in VALID_TEST_CASE_TYPES:
            self.errors.append(f"Row {row_num}: Invalid Test Case Type '{test_type}'. Valid types: {', '.join(VALID_TEST_CASE_TYPES)}")
        
        # Check Expected Output
        expected = row.get("Expected Output", "").strip()
        if not expected:
            self.errors.append(f"Row {row_num}: Expected Output is empty.")
        
        # Check Automated (Yes/No)
        automated = row.get("Automated (Yes/No)", "").strip()
        if not automated:
            self.errors.append(f"Row {row_num}: Automated (Yes/No) is empty.")
        elif automated not in VALID_AUTOMATED:
            self.errors.append(f"Row {row_num}: Invalid Automated value '{automated}'. Must be 'Yes' or 'No'.")
        
        # Actual Output can be "Pending" or empty during test design
        actual = row.get("Actual Output", "").strip()
        if actual and actual not in ["Pending", "Pass", "Fail"]:
            self.warnings.append(f"Row {row_num}: Unusual Actual Output value '{actual}'. Typically 'Pending', 'Pass', or 'Fail'.")
    
    def _check_requirement_coverage(self):
        """Check if all required requirements are covered by at least one test."""
        all_expected = set()
        for req_type, req_nums in EXPECTED_REQUIREMENTS.items():
            for num in req_nums:
                all_expected.add(f"{req_type}-{num}")
        
        missing = all_expected - self.covered_requirements
        if missing:
            self.errors.append(
                f"Missing test coverage for requirements: {', '.join(sorted(missing))}\n"
                f"Covered: {', '.join(sorted(self.covered_requirements))}"
            )
        else:
            print(f"✓ All requirements covered: {len(self.covered_requirements)} tests", file=sys.stderr)


def main():
    if len(sys.argv) < 2:
        print("Usage: python validate-csv.py <path-to-testspec.csv>", file=sys.stderr)
        sys.exit(1)
    
    csv_path = sys.argv[1]
    validator = CSVValidator(csv_path)
    is_valid, errors, warnings = validator.validate()
    
    if errors:
        print("\n❌ VALIDATION ERRORS:", file=sys.stderr)
        for error in errors:
            print(f"  • {error}", file=sys.stderr)
    
    if warnings:
        print("\n⚠️  VALIDATION WARNINGS:", file=sys.stderr)
        for warning in warnings:
            print(f"  • {warning}", file=sys.stderr)
    
    if is_valid:
        print("✅ CSV validation passed!", file=sys.stderr)
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
