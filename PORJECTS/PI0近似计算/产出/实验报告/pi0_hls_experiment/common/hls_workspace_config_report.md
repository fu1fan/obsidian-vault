# HLS Workspace Configuration Report

## Status

This report is superseded. The repository no longer maintains `vitis_workspace/`
as a Vitis Unified IDE workspace.

Active HLS files are now limited to source/testbench/script inputs under
`hls_exp/`. `scripts/run_all_hls.py` generates temporary
self-managed HLS work directories under ignored `hls_exp/.hls_work/`
when C-sim or synthesis is requested.

## Retired Vitis Unified State

The following Vitis Unified IDE/config artifacts were removed from active Git
tracking:

- `vitis_workspace/_ide/`
- `vitis_workspace/config_backups/`
- committed per-kernel component wrapper directories such as
  `vitis_workspace/<kernel>/`
- committed `vitis-comp.json`, `hls_config.cfg`, `main.cpp`, and `test.cpp`
  wrapper files outside `hls_exp/`
- Vitis Unified config maintenance utilities:
  `tools/backup_vitis_workspace_config.py`,
  `tools/validate_vitis_workspace_config.py`, and
  `tools/update_vitis_workspace_components.py`

Historical report copies under `results/hls_reports/` are retained as evidence
for earlier experiments. Some raw logs may mention old component paths because
they are immutable historical synthesis outputs, not active workspace config.

## Current Reproducibility Path

Use the self-managed runner:

```bash
python scripts/run_all_hls.py --skip-synth
python scripts/run_all_hls.py --kernels fixed_projector_tile_opt_p16k8
python scripts/parse_hls_reports.py
python scripts/summarize_hls_results.py
python scripts/summarize_hls_latency_optimization.py
```

The generated temporary work dirs are intentionally ignored and can be safely
deleted. Only source files, scripts, CSVs, summaries, and copied report excerpts
belong in Git.
