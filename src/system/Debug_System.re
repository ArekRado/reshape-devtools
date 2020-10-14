let update = (~state: Reshape.Type.state, ~debug: bool): Reshape.Type.state =>
  if (debug) {
    if (!state.isDebugInitialized) {
      SyncState_Util.set(state, Editor);
      Debug_Util.initialize();

      {...state, isDebugInitialized: true};
    } else {
      if(state.time.delta !== 0.0) {
        SyncState_Util.set(state, Editor);
        state;
      } else {
        switch (SyncState_Util.get(Game)) {
        | Some(editorState) => {...editorState, isDebugInitialized: true}
        | None => state
        };
      }
    };
  } else {
    state;
  };
