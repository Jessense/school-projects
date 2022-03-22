program example(input,output);
    const d = +2; e = -2;
    var x, y:integer; 
        z:real;
        temp:integer;
        a, b, c:char;
        h: array[0..10] of integer;
        i: array[0..10] of integer;
        j: array[0..10] of integer;
        k: array[0..20, 0..30] of integer;
        m: array[0..10] of char;

    procedure init(a:integer);
        begin
            if a < 5 then
                a := a + c; {int <- int + char}
            for temp := 0 to 9 do
                begin
                    h[temp] := 2*temp + 2;
                    i[temp] := (temp + a) * 2;
                end;
        end;

    procedure print(a:integer);
        begin
            for temp := 0 to 9 do
                writeln(j[temp]);
        end;

    function gcd(a,b:integer):char; {change to bad return value}
        begin
            if b=0 then exit(a)
            else exit(gcd(b, a mod b))
        end;

    begin
        init(3);
        temp := a; {int <- char}
        while temp <= 9 do
            begin
                j[temp] := gcd(h[temp], i[temp]); {int <- char}
                temp := temp + 1;
            end;
        print(1);
    end.