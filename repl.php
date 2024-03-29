<?php
    $__colours = [
        'NC' => "\033[0m",
        'RED' => "\033[0;31m",
        'GREEN' => "\033[0;32m",
        'LGREEN' => "\033[1;32m",
        'BLUE' => "\033[0;34m",
        'LBLUE' => "\033[1;34m",
        'CYAN' => "\033[0;36m",
        'LCYAN' => "\033[1;36m",
        'YELLOW' => "\033[1;33m"
    ];

    $__readline = function_exists('readline');

    echo "{$__colours['GREEN']}PHP REPL{$__colours['NC']}\n";

    if ($__readline) {
        echo "{$__colours['GREEN']}Using readline{$__colours['NC']}\n";

        readline_completion_function(function ($command, $pos) {
            global $__consts;
            global $__funcs;
            global $__classes;
            global $__interfaces;
            global $__traits;
            global $__vars;

            return array_merge(array_keys($__vars), array_keys($__consts), array_values($__funcs['user']), array_values($__funcs['internal']), array_values($__classes), array_values($__interfaces), array_values($__traits));
        });
    }

    while (true) {
        $__input = '';
        $__line = 0;

        while (true) {
            $__consts = get_defined_constants();
            $__funcs = get_defined_functions();
            $__classes = get_declared_classes();
            $__interfaces = get_declared_interfaces();
            $__traits = get_declared_traits();
            $__vars = get_defined_vars();

            $__line_count = $__line > 0 ? (string)$__line : '';
            $__line_prompt = "{$__colours['GREEN']}\n{$__line_count}> ";

            if ($__readline) {
                $__input_line = readline($__line_prompt);

                if (trim($__input_line) !== '') {
                    readline_add_history($__input_line);
                }
            } else {
                echo $__line_prompt;
                $__input_line = fgets(STDIN);
            }

            $__line++;

            if (($__input_line[0] ?? '') === ' ') {
                $__input_line = substr($__input_line, 1);
                $__input .= $__input_line;
                continue;
            }

            $__input .= $__input_line;
            break;
        }

        echo $__colours['NC'];

        if (trim($__input_line) === 'exit') {
            exit;
        }

        try {
            eval($__input);
        } catch (Throwable $e) {
            echo "{$__colours['RED']}ERROR: ".$e->getMessage().$__colours['NC'];
        }
    }

