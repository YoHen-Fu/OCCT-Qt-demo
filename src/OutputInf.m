function OutputInf = OutputInf(Value, Inf)
    OutputInf = ["[" + string(datestr(now,31)) + "]" + Inf; Value];
end