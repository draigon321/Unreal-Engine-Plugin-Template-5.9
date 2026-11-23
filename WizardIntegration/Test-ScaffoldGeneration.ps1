param(
    [Parameter(Mandatory)] [string] $OutputDirectory
)

$ErrorActionPreference = 'Stop'
$root = Join-Path $PSScriptRoot 'UE59PluginWizard/Templates'
$options = @('ActorComponent', 'Interface', 'DataAsset', 'FunctionLibrary')
$output = [IO.Path]::GetFullPath($OutputDirectory)
if (Test-Path -LiteralPath $output) { throw "Use a new output directory: $output" }

# Mirror PluginUtils' filename, class and API macro replacement for the source
# fixtures. This is a static regression test, not an editor wizard simulation.
foreach ($mask in 0..15) {
    $name = "ScaffoldTest$mask"
    $destination = Join-Path $output $name
    $folders = @((Join-Path $root 'RuntimeEditor'))
    for ($i = 0; $i -lt $options.Count; $i++) {
        if ($mask -band (1 -shl $i)) { $folders += Join-Path $root "Scaffolds/$($options[$i])" }
    }
    foreach ($folder in $folders) {
        foreach ($file in Get-ChildItem -LiteralPath $folder -File -Recurse) {
            $relative = $file.FullName.Substring($folder.Length).TrimStart('\', '/')
            $target = Join-Path $destination ($relative.Replace('PLUGIN_NAME', $name))
            if (Test-Path -LiteralPath $target) { throw "Duplicate target: $target" }
            New-Item -ItemType Directory -Path (Split-Path $target) -Force | Out-Null
            $content = (Get-Content -LiteralPath $file.FullName -Raw).Replace('PLUGIN_NAME', $name).Replace("${name}_API", "${name}_API".ToUpperInvariant())
            if ($content.Contains('PLUGIN_NAME')) { throw "Unsubstituted token: $target" }
            Set-Content -LiteralPath $target -Value $content -Encoding utf8
        }
    }
    $descriptor = @{ FileVersion = 3; Version = 1; VersionName = '1.0'; FriendlyName = $name; CanContainContent = $true; Modules = @(
        @{ Name = $name; Type = 'Runtime'; LoadingPhase = 'Default' },
        @{ Name = "${name}Editor"; Type = 'Editor'; LoadingPhase = 'Default' }
    ) }
    $descriptor | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $destination "$name.uplugin") -Encoding utf8
    $suffixes = @('Component', 'Interface', 'DataAsset', 'FunctionLibrary')
    for ($i = 0; $i -lt $suffixes.Count; $i++) {
        $exists = Test-Path -LiteralPath (Join-Path $destination "Source/$name/Public/$name$($suffixes[$i]).h")
        if ($exists -ne [bool]($mask -band (1 -shl $i))) { throw "Unexpected scaffold selection: $name/$($suffixes[$i])" }
    }
    Write-Host "${name}: mask $mask passed"
}
