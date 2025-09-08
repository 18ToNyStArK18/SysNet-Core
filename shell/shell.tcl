#!/usr/bin/expect -f

set shell_executable "./shell.out"
set timeout 3

if {![info exists env(USER)]} {
    set env(USER) [exec id -un]
}
if {![info exists env(HOSTNAME)]} {
    set env(HOSTNAME) [exec hostname]
}

set user $env(USER)
set hostname $env(HOSTNAME)
set shell_prompt_regex "<${user}@${hostname}:\[^>]+> "

set PASS_COUNT 0
set FAIL_COUNT 0
set TEMP_DIR ".test"

array set style {
    header  "\033\[1;35m"
    group   "\033\[0;35m"
    test    "\033\[0;33m"
    pass    "\033\[0;32m"
    fail    "\033\[0;31m"
    info    "\033\[0;37m"
    dim     "\033\[2;37m"
    reset   "\033\[0m"
}

proc cleanup {} {
    global TEMP_DIR
    catch {exec rm -rf $TEMP_DIR}
}

proc print_header {title} {
    global style
    set separator [string repeat "=" [expr {[string length $title] + 20}]]
    puts "\n$style(header)shell test suite - [string tolower $title]$style(reset)"
    puts "$style(header)$separator$style(reset)"
}

proc print_section {title} {
    global style
    set separator [string repeat "-" [expr {[string length $title] + 3}]]
    puts "\n$style(group)>> [string tolower $title]$style(reset)"
    puts "$style(group)$separator$style(reset)"
}

proc print_test_result {test_desc status {details ""}} {
    global style PASS_COUNT FAIL_COUNT

    if {$status == "pass"} {
        puts "$style(pass)pass$style(reset) | [string tolower $test_desc]"
        incr PASS_COUNT
    } else {
        puts "$style(fail)fail$style(reset) | [string tolower $test_desc]"
        if {$details != ""} {
            regsub -all {[\x00-\x1F\x7F]} $details {.} sanitized_details
            puts "$style(dim)    details: $sanitized_details$style(reset)"
        }
        incr FAIL_COUNT
    }
}

proc check_file_content {test_desc file expected_pattern} {
    if {![file exists $file]} {
        print_test_result $test_desc "fail" "file '$file' was not created."
        return
    }

    set fd [open $file r]
    set actual_content [read $fd]
    close $fd

    regsub -all {\s+} [string trim $actual_content] " " single_line_content

    if {[string match $expected_pattern $single_line_content]} {
        print_test_result $test_desc "pass"
    } else {
        print_test_result $test_desc "fail" \
            "expected pattern '$expected_pattern', got '$single_line_content'"
    }
}

proc run_test {test_desc setup_cmds test_steps} {
    global shell_executable shell_prompt_regex timeout style

    puts "$style(test)test$style(reset) | [string tolower $test_desc]"

    if {$setup_cmds != ""} {
        if {[catch {exec sh -c $setup_cmds} setup_error]} {
            puts "$style(dim)    setup warning: $setup_error$style(reset)"
        }
    }

    log_user 0
    spawn -noecho $shell_executable

    expect {
        -re $shell_prompt_regex {}
        timeout {
            print_test_result $test_desc "fail" "timeout waiting for initial shell prompt."
            catch {close}; catch {wait}
            log_user 1
            return
        }
        eof {
            print_test_result $test_desc "fail" "shell exited unexpectedly on startup."
            catch {wait}
            log_user 1
            return
        }
    }

    set success 1
    foreach step $test_steps {
        set cmd [lindex $step 0]
        set pattern [lindex $step 1]
        send -- "$cmd\r"

        expect {
            -re $pattern {}
            timeout {
                print_test_result $test_desc "fail" \
                    "timeout after command: '$cmd'. expected pattern: '$pattern'"
                set success 0
                break
            }
            eof {
                print_test_result $test_desc "fail" \
                    "shell exited unexpectedly after command: '$cmd'."
                set success 0
                break
            }
        }
    }

    send "\x04"
    expect {
        "logout" {}
        timeout {
            puts "warning: exiting did not print 'logout' or timed out."
        }
    }
    expect eof
    catch {wait}
    log_user 1

    if {$success} {
        print_test_result $test_desc "pass"
    }
}

proc run_job_control_test {} {
    global shell_executable shell_prompt_regex style

    puts "$style(test)test$style(reset) | sleep, activities, bg, fg"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex

    send "sleep 7 &\r"
    expect -re {\s*\[1\] \d+\s*}
    expect -re $shell_prompt_regex

    send "activities\r"
    expect -re ".*sleep - Running"
    expect -re $shell_prompt_regex

    send "sleep 5\r"
    sleep 0.5
    send "\x1a"
    expect -re ".*Stopped sleep"
    expect -re $shell_prompt_regex

    send "activities\r"
    expect -re ".*sleep - Running.*sleep - Stopped"
    expect -re $shell_prompt_regex

    send "bg 1\r"
    expect -re "Job already running"
    expect -re $shell_prompt_regex

    send "bg 2\r"
    expect -re ".*sleep &"
    expect -re $shell_prompt_regex

    send "fg 1\r"
    set timeout 12
    expect -re $shell_prompt_regex {
        set timeout 3
        send "fg 2\r"
        sleep 0.5
        send "\x03"
        expect -re $shell_prompt_regex {
            print_test_result "sleep, activities, bg, fg" "pass"
        } timeout {
            print_test_result "sleep, activities, bg, fg" "fail" \
                "shell did not return to prompt after ctrl+c"
        }
    } timeout {
        print_test_result "sleep, activities, bg, fg" "fail" \
            "fg command did not complete as expected"
    }

    set timeout 3
    send "\x04"
    expect eof
    catch {wait}
    log_user 1
}

proc run_async_notification_test {} {
    global shell_executable shell_prompt_regex style

    puts "$style(test)test$style(reset) | job completion"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex

    send "sleep 1 &\r"
    expect -re {\s*\[1\] \d+\s*}
    expect -re $shell_prompt_regex

    sleep 2
    send "echo trigger\r"
    expect {
        -re "(?s).*sleep with pid.*exited normally.*trigger.*$shell_prompt_regex" {
            print_test_result "job completion" "pass"
        }
        timeout {
            print_test_result "job completion" "fail" \
                "did not receive job completion message."
        }
    }

    send "\x04"
    expect eof
    catch {wait}
    log_user 1
}

proc run_echo_tests {} {
    print_section "echo builtin"

    run_test "echo simple text" "" \
        [list [list "echo hello world" "hello world"]]

    run_test "echo empty string" "" \
        [list [list "echo \"\"" ""]]

    run_test "echo multiple arguments" "" \
        [list [list "echo one two three" "one two three"]]
}

proc run_shell_behavior_tests {} {
    print_section "shell behavior"

    run_test "empty command" "" \
        [list [list "" ""]]

    run_test "whitespace only command" "" \
        [list [list "   " ""]]

    run_test "command not found" "" \
        [list [list "nonexistentcommand" "Command not found"]]

    run_test "command with invalid path" "" \
        [list [list "/nonexistent/command" "Command not found"]]
}

proc run_hop_basic_tests {} {
    global TEMP_DIR

    print_section "hop (cd) basic functionality"

    run_test "hop without arguments" "" \
        [list [list "hop" ""]]

    run_test "hop with multiple arguments" "" \
        [list [list "hop . .." ""]]

    run_test "hop to nonexistent directory" "" \
        [list [list "hop /nonexistent/dir" "No such directory!"]]
}

proc run_hop_prompt_tests {} {
    global shell_executable shell_prompt_regex TEMP_DIR style

    puts "$style(test)test$style(reset) | hop should update prompt"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex

    send "hop $TEMP_DIR\r"
    expect -re ".*$TEMP_DIR> " {
        send "pwd\r"
        expect -re ".*$TEMP_DIR" {
            send "hop -\r"
            expect -re ".*~> " {
                print_test_result "hop should update prompt" "pass"
            } timeout {
                print_test_result "hop should update prompt" "fail" \
                    "prompt did not revert after 'hop -'"
            }
        } timeout {
            print_test_result "hop should update prompt" "fail" \
                "pwd command failed or timed out"
        }
    } timeout {
        print_test_result "hop should update prompt" "fail" \
            "prompt did not update after 'hop' or command timed out."
    }

    send "\x04"
    expect eof
    catch {wait}
    log_user 1
}

proc run_reveal_alias_tests {} {
    global shell_executable shell_prompt_regex TEMP_DIR style

    puts "$style(test)test$style(reset) | reveal in current and old pwd"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex

    send "hop $TEMP_DIR\r"
    expect -re $shell_prompt_regex
    send "reveal\r"
    expect -re ".*f1.*f2.*" {
        expect -re $shell_prompt_regex
        send "hop ..\r"
        expect -re $shell_prompt_regex
        send "reveal -\r"
        expect -re ".*f1.*f2.*" {
            print_test_result "reveal in current and old pwd" "pass"
        } timeout {
            print_test_result "reveal in current and old pwd" "fail" \
                "reveal did not list files in prev pwd"
        }
    } timeout {
        print_test_result "reveal in current and old pwd" "fail" \
            "reveal did not list files in pwd"
    }

    send "\x04"
    expect eof
    catch {wait}
    log_user 1
}

proc run_reveal_basic_tests {} {
    global TEMP_DIR

    print_section "reveal (ls) basic functionality"

    run_test "reveal basic" "touch $TEMP_DIR/f1 $TEMP_DIR/f2" \
        [list [list "reveal $TEMP_DIR" ".*f1.*f2.*"]]

    run_test "reveal nonexistent directory" "" \
        [list [list "reveal /nonexistent" "No such directory!"]]
}

proc run_reveal_options_tests {} {
    global TEMP_DIR

    print_section "reveal (ls) with options"

    run_test "reveal hidden files (-a)" "touch $TEMP_DIR/.hidden" \
        [list [list "reveal -a $TEMP_DIR" ".*\\.hidden.*"]]

    run_test "reveal long format (-l)" "touch $TEMP_DIR/f3" \
        [list [list "reveal -l $TEMP_DIR" ".*f3.*"]]

    run_test "reveal combined flags (-la)" "touch $TEMP_DIR/.hidden2" \
        [list [list "reveal -la $TEMP_DIR" ".*\\.hidden2.*"]]
}

proc run_output_redirect_tests {} {
    global TEMP_DIR
    set REDIRECT_FILE "$TEMP_DIR/out.txt"

    print_section "output redirection (>)"

    run_test "basic output redirect" "" \
        [list [list "echo content1 > $REDIRECT_FILE" ""]]
    check_file_content "basic output redirect content" $REDIRECT_FILE "content1"

    run_test "output overwrite" "echo old > $REDIRECT_FILE" \
        [list [list "echo new > $REDIRECT_FILE" ""]]
    check_file_content "output overwrite content" $REDIRECT_FILE "new"

    run_test "redirect with true command" "" \
        [list [list "/bin/true > $REDIRECT_FILE" ""]]
    check_file_content "true command redirect content" $REDIRECT_FILE ""

    run_test "redirect multiple words" "" \
        [list [list "echo word1 word2 word3 > $REDIRECT_FILE" ""]]
    check_file_content "multiple words redirect" $REDIRECT_FILE "word1 word2 word3"
}

proc run_append_redirect_tests {} {
    global TEMP_DIR
    set REDIRECT_FILE "$TEMP_DIR/append.txt"

    print_section "append redirection (>>)"

    run_test "basic append redirect" "echo line1 > $REDIRECT_FILE" \
        [list [list "echo line2 >> $REDIRECT_FILE" ""]]
    check_file_content "basic append content" $REDIRECT_FILE "line1 line2"

    run_test "append to nonexistent file" "rm -f $REDIRECT_FILE" \
        [list [list "echo newfile >> $REDIRECT_FILE" ""]]
    check_file_content "append new file content" $REDIRECT_FILE "newfile"

    run_test "multiple appends" "echo first > $REDIRECT_FILE" \
        [list \
            [list "echo second >> $REDIRECT_FILE" ""] \
            [list "echo third >> $REDIRECT_FILE" ""] \
        ]
    check_file_content "multiple appends content" $REDIRECT_FILE "first second third"
}

proc run_input_redirect_tests {} {
    global TEMP_DIR
    set INPUT_FILE "$TEMP_DIR/input.txt"

    print_section "input redirection (<)"

    run_test "basic input redirect" "echo 12345 > $INPUT_FILE" \
        [list [list "wc -c < $INPUT_FILE" "6"]]

    run_test "input redirect with cat" "echo 'hello world' > $INPUT_FILE" \
        [list [list "cat < $INPUT_FILE" "hello world"]]

    run_test "input redirect nonexistent file" "" \
        [list [list "cat < /nonexistent/file" "No such file or directory!"]]

    run_test "input redirect with wc lines" "printf 'line1\nline2\nline3\n' > $INPUT_FILE" \
        [list [list "wc -l < $INPUT_FILE" "3"]]
}

proc run_builtin_redirect_tests {} {
    global TEMP_DIR
    set REDIRECT_FILE "$TEMP_DIR/builtin.txt"

    print_section "builtin redirection"

    run_test "redirect reveal output" "" \
        [list [list "reveal > $REDIRECT_FILE" ""]]
    check_file_content "reveal redirect content" $REDIRECT_FILE "*akefile*"

    run_test "redirect echo output" "" \
        [list [list "echo builtin_test > $REDIRECT_FILE" ""]]
    check_file_content "echo redirect content" $REDIRECT_FILE "builtin_test"

    run_test "redirect activities output" "sleep 1 &" \
        [list [list "activities > $REDIRECT_FILE" ""]]
}

proc run_pipe_basic_tests {} {
    global TEMP_DIR
    print_section "basic pipes"

    run_test "echo to wc" "" \
        [list [list "echo 'one two three' | wc -w" "3"]]

    run_test "echo to grep" "" \
        [list [list "echo 'hello world' | grep hello" "hello world"]]

    run_test "cat to wc" "echo 'line1\nline2' > $TEMP_DIR/pipe_test.txt" \
        [list [list "cat $TEMP_DIR/pipe_test.txt | wc -l" "2"]]

    run_test "reveal to grep" "touch $TEMP_DIR/test_file" \
        [list [list "reveal -l $TEMP_DIR | grep test_file" "test_file"]]
}

proc run_pipe_chain_tests {} {
    global TEMP_DIR
    print_section "pipe chains"

    run_test "three command pipe" "" \
        [list [list "echo apple banana cherry | tac | wc -w" "3"]]

    run_test "pipe builtin to external" "touch $TEMP_DIR/1pipe $TEMP_DIR/2pipe" \
        [list [list "reveal -l $TEMP_DIR | head -2" ".*1pipe.*2pipe.*"]]

    run_test "pipe to invalid command" "" \
        [list [list "echo test | invalidcommand" "Command not found"]]
}

proc run_sequential_tests {} {
    print_section "sequential operator"

    run_test "two commands sequential" "" \
        [list [list "echo first; echo second" "first\r\nsecond"]]

    run_test "three commands sequential" "" \
        [list [list "echo one; echo two; echo three" "one\r\ntwo\r\nthree"]]

    run_test "sequential with failure" "" \
        [list [list "/bin/false; echo after_false" "after_false"]]

    run_test "sequential builtin commands" "" \
        [list [list "echo start; hop ~; echo end" "start\r\nend"]]
}

proc run_conditional_tests {} {
    print_section "conditional operator (&&)"

    run_test "conditional success" "" \
        [list [list "echo first && echo second" "first\r\nsecond"]]

    run_test "conditional failure" "" \
        [list [list "/bin/false && echo second" ""]]

    run_test "conditional chain success" "" \
        [list [list "/bin/true && echo middle && echo end" "middle\r\nend"]]

    run_test "conditional mixed with sequential" "" \
        [list [list "/bin/false && echo skip; echo always" "always"]]
}

proc run_job_background_tests {} {
    print_section "background jobs"

    run_job_control_test
    run_async_notification_test
}

proc run_job_management_tests {} {
    global shell_executable shell_prompt_regex style

    print_section "job management commands"

    puts "$style(test)test$style(reset) | activities with no jobs"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex
    send "activities\r"
    expect -re $shell_prompt_regex {
        print_test_result "activities with no jobs" "pass"
    } timeout {
        print_test_result "activities with no jobs" "fail" "timeout waiting for activities command"
    }

    puts "$style(test)test$style(reset) | bg and fg with invalid job"
    send "bg 8\r"
    expect -re "No such job" {
        print_test_result "bg with invalid job" "pass"
    } timeout {
        print_test_result "bg with invalid job" "fail" "incorrect error message"
    }
    expect -re $shell_prompt_regex

    send "fg 8\r"
    expect -re "No such job" {
        print_test_result "fg with invalid job" "pass"
    } timeout {
        print_test_result "fg with invalid job" "fail" "incorrect error message"
    }
    expect -re $shell_prompt_regex

    send "\x04"; expect eof; catch {wait}
    log_user 1
}

proc run_signal_tests {} {
    global shell_executable shell_prompt_regex style

    print_section "signal handling"

    puts "$style(test)test$style(reset) | ctrl+c interrupt foreground job"
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex
    send "sleep 5\r"
    sleep 0.5
    send "\x03"
    expect -re $shell_prompt_regex {
        print_test_result "ctrl+c interrupt foreground job" "pass"
    } timeout {
        print_test_result "ctrl+c interrupt foreground job" "fail" "shell did not return to prompt after interrupt"
    }
    send "\x04"; expect eof; catch {wait}
    log_user 1

    puts "$style(test)test$style(reset) | ctrl+z suspend foreground job"
    set timeout 12
    log_user 0
    spawn -noecho $shell_executable
    expect -re $shell_prompt_regex
    send "sleep 5\r"
    sleep 1
    send "\x1a"
    expect -re ".*Stopped.*" {
        expect -re $shell_prompt_regex {
            print_test_result "ctrl+z suspend foreground job" "pass"
        } timeout {
            print_test_result "ctrl+z suspend foreground job" "fail" "prompt not returned after suspend"
        }
    } timeout {
        print_test_result "ctrl+z suspend foreground job" "fail" "no suspend message received"
    }
    send "\x04"; expect eof; catch {wait}
    set timeout 3
    log_user 1
}

proc run_history_basic_tests {} {
    print_section "log (history) basic"

    run_test "log empty history" "" \
        [list [list "log" ""]]

    run_test "log single command" "" [list \
        [list "echo test_cmd" "test_cmd"] \
        [list "log" "echo test_cmd"] \
    ]

    run_test "log multiple commands" "" [list \
        [list "echo cmd1" "cmd1"] \
        [list "echo cmd2" "cmd2"] \
        [list "log" "echo cmd1\r\necho cmd2"] \
    ]

    run_test "do not log same command" "" [list \
        [list "echo cmd1" "cmd1"] \
        [list "echo cmd1" "cmd1"] \
        [list "log" "echo cmd1"] \
    ]
}

proc run_history_execute_tests {} {
    print_section "log (history) execute"

    run_test "log execute recent command" "" [list \
        [list "echo unique_cmd" "unique_cmd"] \
        [list "log execute 1" "unique_cmd"] \
    ]

    run_test "log execute with multiple history" "" [list \
        [list "echo first" "first"] \
        [list "echo second" "second"] \
        [list "log execute 1" "second"] \
        [list "log execute 2" "first"] \
    ]
}

proc run_history_management_tests {} {
    print_section "log (history) management"

    run_test "log purge clears history" "" [list \
        [list "echo something" "something"] \
        [list "log purge" ""] \
        [list "log" ""] \
    ]

    run_test "log purge then new commands" "" [list \
        [list "echo old" "old"] \
        [list "log purge" ""] \
        [list "echo new" "new"] \
        [list "log" "echo new"] \
    ]
}

proc run_complex_command_tests {} {
    global TEMP_DIR

    print_section "complex command combinations"

    run_test "redirect and pipe combination" "" \
        [list [list "echo 'data' > $TEMP_DIR/pipe_redir.txt ; cat $TEMP_DIR/pipe_redir.txt | wc -c" "5"]]

    run_test "background with redirect" "" \
        [list [list "echo background_test > $TEMP_DIR/bg_test.txt &" ""]]

    run_test "builtin in pipe chain" "touch $TEMP_DIR/builtin_pipe" \
        [list [list "reveal $TEMP_DIR | grep builtin_pipe | wc -l" "1"]]
}

proc print_results {} {
    global PASS_COUNT FAIL_COUNT style

    cleanup
    print_header "results"

    set total_tests [expr $PASS_COUNT + $FAIL_COUNT]
    if {$total_tests == 0} {
        puts "$style(fail)no tests were run.$style(reset)"
        exit 1
    }

    set pass_rate [expr {($PASS_COUNT * 100.0) / $total_tests}]

    puts ""
    puts "$style(pass)  passed: $PASS_COUNT$style(reset)"
    puts "$style(fail)  failed: $FAIL_COUNT$style(reset)"
    puts "$style(info)  total:  $total_tests$style(reset)"
    puts [format "$style(header)  success rate: %.1f%%$style(reset)" $pass_rate]
    puts ""

    if {$FAIL_COUNT > 0} {
        puts "$style(fail)some tests failed - review output above.$style(reset)"
        exit 1
    } else {
        puts "$style(pass)all tests passed successfully!$style(reset)"
    }
    exit 0
}

proc main {} {
    global shell_executable TEMP_DIR style

    cleanup
    print_header "running tests"

    if {![file executable $shell_executable]} {
        puts "$style(test)info$style(reset) | shell not found, attempting 'make all'..."
        if {[catch {exec make all} result]} {
            print_test_result "compilation" "fail" $result
            exit 1
        }
        print_test_result "compilation" "pass"
    }

    exec mkdir -p $TEMP_DIR

    run_echo_tests
    run_shell_behavior_tests
    run_hop_basic_tests
    run_hop_prompt_tests
    run_reveal_basic_tests
    run_reveal_options_tests
    run_reveal_alias_tests
    run_output_redirect_tests
    run_append_redirect_tests
    run_input_redirect_tests
    run_builtin_redirect_tests
    run_pipe_basic_tests
    run_pipe_chain_tests
    run_sequential_tests
    # run_conditional_tests
    run_job_background_tests
    run_job_management_tests
    run_signal_tests
    run_history_basic_tests
    run_history_execute_tests
    run_history_management_tests
    run_complex_command_tests

    print_results
}

main
