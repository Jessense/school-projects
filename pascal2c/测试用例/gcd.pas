program example(input,output);
    var x, y, z:integer;
    function gcd(a,b:integer):integer;
    begin
        if b=0 then exit(a)
        else exit(gcd(b, a mod b))
    end;
    begin
        x := 24;
        y := 18;
        z := gcd(x, y);
        writeln(z);
    end.