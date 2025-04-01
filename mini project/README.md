CLASS LoopSyntaxAnalyzer:
    STRUCT LoopInfo:
        type: String                // "for", "while", "do-while"
        initialization: String      // For loops
        condition: String           // All loops
        iteration: String           // For loops
        lineNumber: Integer         // Line where loop starts
        isValid: Boolean            // Syntax validity
        errorMessage: String        // Description if invalid

    sourceCode: Array of Strings
    loops: Array of LoopInfo
    
    // Helper functions
    FUNCTION trim(text):
        Return text with whitespace removed from start and end
    
    FUNCTION hasBalancedParentheses(text):
        Use stack to check if parentheses are balanced
        Return true if balanced, false otherwise
    
    FUNCTION extractParenthesesContent(text):
        Extract content between outermost parentheses
    
    // Main analysis function
    FUNCTION analyze():
        Clear previously found loops
        
        // Track line positions in concatenated text
        Create array of line starting positions
        
        // Concatenate all code into one string for regex processing
        concatenatedCode = All source lines joined with newlines
        
        // Create helper function to find line number from position
        FUNCTION getLineNumber(position):
            Find the line containing position using binary search
            Return line number (1-indexed)
        
        // Track processed positions to avoid duplicates
        Create set to store processed positions
        
        // 1. Process standard for loops (for(init;cond;iter))
        FOR each match of regex "for\s*\(\s*([^;]*?)\s*;\s*([^;]*?)\s*;\s*([^;]*?)\s*\)":
            Skip if position already processed
            Skip if inside string or comment
            
            Create new LoopInfo
            Set type to "for"
            Set line number based on match position
            Set valid to true initially
            
            Extract initialization, condition, and iteration parts
            
            IF parentheses not balanced:
                Mark as invalid with error message
            
            Add to loops array
            Mark position as processed
        
        // 2. Process range-based for loops (for(var : container))
        FOR each match of regex "for\s*\(\s*([^:]*?)\s*:\s*([^)]*?)\s*\)":
            Skip if position already processed
            Skip if inside string or comment
            
            Create new LoopInfo
            Set type to "for"
            Set line number based on match position
            Set valid to true
            Mark as range-based loop
            
            Extract variable and container parts
            
            IF parentheses not balanced:
                Mark as invalid with error message
            
            Add to loops array
            Mark position as processed
        
        // 3. Process invalid for loops (missing semicolons)
        FOR each match of regex "for\s*\(\s*([^;:)]*)\s*\)":
            Skip if position already processed
            Skip if matches valid for loop patterns
            Skip if inside string or comment
            
            Create new LoopInfo
            Set type to "for"
            Set line number based on match position
            Set valid to false
            Set error message about missing semicolons
            
            Add to loops array
            Mark position as processed
        
        // 4. Process while loops
        FOR each match of regex "while\s*\(\s*(.+?)\s*\)":
            Skip if position already processed
            Skip if inside string or comment
            Skip if part of do-while loop
            
            Create new LoopInfo
            Set type to "while"
            Set line number based on match position
            Set valid to true
            
            Extract condition
            
            IF condition empty OR parentheses not balanced:
                Mark as invalid with appropriate error message
            
            Add to loops array
            Mark position as processed
        
        // 5. Process do-while loops
        FOR each match of regex "\bdo\s*\{":
            Skip if position already processed
            Skip if inside string or comment
            
            Find matching closing brace with balanced braces
            Look for "while" after closing brace
            
            IF found "while" with parenthesized condition:
                Create new LoopInfo
                Set type to "do-while"
                Set line number based on match position
                
                Extract condition from parentheses
                Check for semicolon after condition
                
                IF no semicolon:
                    Mark as invalid with error message
                IF condition empty:
                    Mark as invalid with error message
                
                Add to loops array
                Mark position as processed

    FUNCTION displayResults():
        Output summary statistics (total loops, valid vs invalid)
        
        Print table headers
        FOR each loop in loops:
            Print line number, type, status, error message
            IF valid:
                Print loop components based on type
            Print separator line

MAIN PROCEDURE:
    Create analyzer instance
    Display program title
    
    LOOP until exit:
        Display menu options
        Get user choice
        
        IF choice is analyze file:
            Prompt for filename
            IF file loaded successfully:
                Analyze the file
                Display results
        ELSE IF choice is exit:
            Exit program
        ELSE:
            Display error message