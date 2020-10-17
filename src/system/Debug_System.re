let update = (~state: Reshape.Type.state): Reshape.Type.state =>
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
