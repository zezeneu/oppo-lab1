# Creates big.txt in Windows-1251 (300000 valid lesson lines + filter line).
# ASCII-only script: Cyrillic names are built from \u escapes, so it works in any PowerShell.
# Run: powershell -ExecutionPolicy Bypass -File make_big_input.ps1
$names = @([regex]::Unescape("\u0418\u0432\u0430\u043D\u043E\u0432 \u0418. \u0418."), [regex]::Unescape("\u041F\u0435\u0442\u0440\u043E\u0432 \u041F. \u041F."), [regex]::Unescape("\u0421\u0438\u0434\u043E\u0440\u043E\u0432\u0430 \u0410. \u0412."), [regex]::Unescape("\u041A\u0443\u0437\u043D\u0435\u0446\u043E\u0432 \u0414. \u0421."), [regex]::Unescape("\u0421\u043C\u0438\u0440\u043D\u043E\u0432\u0430 \u0415. \u0410."))
$rnd = New-Object System.Random 5
$lines = New-Object System.Collections.Generic.List[string]
for ($i = 0; $i -lt 300000; $i++) {
  $lines.Add(('{0:0000}.{1:00}.{2:00} {3:00}:{4:00} "{5}"' -f $rnd.Next(2020, 2027), $rnd.Next(1, 13), $rnd.Next(1, 29), $rnd.Next(0, 24), $rnd.Next(0, 60), $names[$rnd.Next(0, 5)]))
}
$lines.Add("")
$lines.Add($names[1])
[System.IO.File]::WriteAllLines("$PWD\big.txt", $lines, [System.Text.Encoding]::GetEncoding(1251))
Write-Host "big.txt created"
