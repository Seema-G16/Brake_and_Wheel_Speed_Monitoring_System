#!/bin/bash
###############################################################################
# UT Test Generation Orchestration Script
# 
# Orchestrates the complete workflow: validate CSV → auto-fix → generate tests → build → run
# 
# Usage: bash run-ut-generation.sh [testspec-csv-path] [build-dir]
###############################################################################

set -e  # Exit on error

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'  # No Color

# Defaults
TESTSPEC_CSV="${1:-Docs/ut-test-design/testspec.csv}"
BUILD_DIR="${2:-build}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}UT Test Generation Workflow${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Step 1: Check if testspec.csv exists
echo -e "${YELLOW}[Step 1] Checking testspec.csv...${NC}"
if [ ! -f "$TESTSPEC_CSV" ]; then
    echo -e "${RED}✗ CSV file not found: $TESTSPEC_CSV${NC}"
    exit 1
fi
echo -e "${GREEN}✓ CSV file found: $TESTSPEC_CSV${NC}"
echo ""

# Step 2: Validate CSV
echo -e "${YELLOW}[Step 2] Validating CSV format & coverage...${NC}"
if python3 "$SCRIPT_DIR/validate-csv.py" "$TESTSPEC_CSV"; then
    echo -e "${GREEN}✓ CSV validation passed!${NC}"
    VALIDATION_PASSED=true
else
    echo -e "${YELLOW}⚠ CSV validation failed. Attempting auto-fix...${NC}"
    VALIDATION_PASSED=false
fi
echo ""

# Step 3: Auto-fix CSV (if validation failed)
if [ "$VALIDATION_PASSED" = false ]; then
    echo -e "${YELLOW}[Step 3] Running CSV auto-fix (max 3 attempts)...${NC}"
    if python3 "$SCRIPT_DIR/auto-fix-csv.py" "$TESTSPEC_CSV" 3; then
        echo -e "${GREEN}✓ CSV auto-fix completed. Re-validating...${NC}"
        
        # Re-validate after auto-fix
        if python3 "$SCRIPT_DIR/validate-csv.py" "$TESTSPEC_CSV"; then
            echo -e "${GREEN}✓ CSV now passes validation!${NC}"
        else
            echo -e "${RED}✗ CSV still has validation errors after auto-fix. Manual intervention needed.${NC}"
            exit 1
        fi
    else
        echo -e "${RED}✗ CSV auto-fix failed with errors. Manual intervention needed.${NC}"
        exit 1
    fi
else
    echo -e "${YELLOW}[Step 3] Skipping auto-fix (CSV already valid).${NC}"
fi
echo ""

# Step 4: Configure CMake
echo -e "${YELLOW}[Step 4] Configuring CMake...${NC}"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

if (cd "$BUILD_DIR" && cmake ..); then
    echo -e "${GREEN}✓ CMake configuration successful!${NC}"
else
    echo -e "${RED}✗ CMake configuration failed.${NC}"
    exit 1
fi
echo ""

# Step 5: Build
echo -e "${YELLOW}[Step 5] Building project...${NC}"
if (cd "$BUILD_DIR" && cmake --build .); then
    echo -e "${GREEN}✓ Build successful!${NC}"
else
    echo -e "${RED}✗ Build failed.${NC}"
    exit 1
fi
echo ""

# Step 6: Run tests
echo -e "${YELLOW}[Step 6] Running tests...${NC}"
if (cd "$BUILD_DIR" && ctest --verbose); then
    echo -e "${GREEN}✓ All tests passed!${NC}"
else
    echo -e "${RED}✗ Some tests failed. Review output above.${NC}"
    exit 1
fi
echo ""

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}✓ UT Test Generation workflow completed successfully!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo "Next steps:"
echo "  1. Review generated test files in tests/unit/"
echo "  2. Check testspec.csv for coverage of all requirements"
echo "  3. Create a requirement-to-test traceability table"
echo ""
