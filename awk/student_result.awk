#!/usr/bin/awk -F, -f
# User-defined function to calculate average marks
function calculate_average(total, subjects) {
    return total / subjects;
}
BEGIN { FS="," }
# Skip the header row
NR > 1 {
    total = $3 + $4 + $5 + $6 + $7;  # Calculate total marks
    avg = calculate_average(total, 5);  # Call the function with total and number of subjects
    print $1, $2, "Total:", total, "Average:", avg;  # Print total and average for each student
}
# System command to print current date after processing the file
END {
    print "Current Date and Time:";
    system("date");  # Execute the system command `date` to show the current date and time
}
