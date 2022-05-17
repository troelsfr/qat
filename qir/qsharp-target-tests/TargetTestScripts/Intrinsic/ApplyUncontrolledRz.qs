// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

namespace QATSample {
    open Microsoft.Quantum.Intrinsic;
    open Microsoft.Quantum.Canon;
    open Microsoft.Quantum.Arrays;
    open Microsoft.Quantum.Measurement;

    @EntryPoint()
    operation Main(): Result
    {
      let theta = 0.5273540462858959;
      use qubit = Qubit();

      ApplyUncontrolledRz(theta, qubit);

      return Zero;
    }
}

