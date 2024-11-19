#!/bin/awk -f

BEGIN {
    FS=":"  # Field separator is colon
    printf "%-10s %-15s %-10s %-10s %-10s %-10s\n", "Roll No.", "Name", "BS", "DA", "HRA", "GS"
    printf "-----------------------------------------------------\n"
}

{
    # Calculate DA as 50% of Basic Salary
    da = $3 * 0.5
    
    # Calculate HRA as 20% of Basic Salary
    hra = $3 * 0.2
    
    # Calculate Gross Salary
    gs = $3 + da + hra
    
    # Print formatted output
    printf "%-10s %-15s %-10.2f %-10.2f %-10.2f %-10.2f\n", $1, $2, $3, da, hra, gs
}

END {
    printf "-----------------------------------------------------\n"
}
