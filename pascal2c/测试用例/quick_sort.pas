program sort (input, output);
    var a: array[0..10] of integer;
        x: integer;
        y: integer;
        m: char;
        z: integer;
    procedure readarray;
        var i: integer;
        begin
            for i:=1 to 9 do read(a[i])
        end;
    procedure exchange (i,j: integer);
        begin
            x:=a[i]; a[i]:=a[j]; a[j]:=x
        end;
    function partition (y,z: integer): integer;
        var i,j: integer; pivot: integer;
        begin
            pivot:=a[y];
            i:=y;
            j:=z;
            while i<j do
                begin
                    while (i<j) and (a[i] <= pivot) do i:=i+1;
                    while (i<j) and (a[j] >= pivot) do j:=j-1;
                    if (i<j) then exchange(i, j);
                end;
            exchange(i, y);
            exit(i);
        end;
    procedure quicksort (m,n:integer);
        var k: integer;
         begin
             if (m >= n) then exit;
             k:=partition(m, n);
             quicksort(m, k-1);
             quicksort(k+1, n)
         end;
    begin
        writeln('Please input a sequence:');
        readarray;
        quicksort(1, 9);
        for x:=1 to 9 do write(a[x])
    end.
