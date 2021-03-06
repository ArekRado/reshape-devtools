[@react.component]
let make = () => {
  let (editorContext, setEditorContext) =
    React.useReducer(Editor_Context.reducer, Editor_Context.initialState);

  let (appContext, setAppContext) =
    React.useReducer(App_Context.reducer, App_Context.initialState);

  let (modalContext, setModalContext) =
    React.useReducer(Modal_Context.reducer, Modal_Context.initialState);

  let isPlayingRef = React.useRef(false);
  isPlayingRef.current = editorContext.isPlaying;

  React.useEffect1(
    () => {
      let id =
        Js.Global.setInterval(
          () =>
            if (isPlayingRef.current) {
              switch (SyncState_Util.get(Editor)) {
              | Some(state) => setAppContext(App_Context.SetState(state))
              | None => ()
              };
            },
          50,
        );

      Some(() => {Js.Global.clearInterval(id)});
    },
    [|editorContext.isPlaying|],
  );

  // Sync once at the saart
  React.useEffect0(() => {
    let initialState =
      switch (SyncState_Util.get(Editor)) {
      | Some(state) => state
      | None => Reshape.Type.initialState
      };

    switch(SyncState_Util.getStateFromLocalStorage(initialState)) {
    | Some(state) => 
      state
      ->App_Context.SetState
      ->setAppContext;
    | None => ()
    };

    Some(() => {()});
  });

  <Editor_Context.Provider value=(editorContext, setEditorContext)>
    <App_Context.Provider value=(appContext, setAppContext)>
      <Modal_Context.Provider value=(modalContext, setModalContext)>
        <div className="text-gray-500 bg-gray-900 w-full h-full flex">
          <div className="flex flex-col flex-1 justify-between py-2 pl-2 pr-1">
            <Button
              onClick={_ =>
                setModalContext(Modal_Context.OpenModal("saveState", None))
              }>
              {React.string("Save")}
            </Button>
            <StartStop />
            <CreateEntity />
            <div className="flex flex-col flex-1 overflow-y-scroll mt-2">
              <EntityList />
            </div>
            <Fps />
          </div>
          <div
            className="flex flex-col flex-1 overflow-y-scroll overflow-x-hidden py-2 pr-2 pl-1">
            {editorContext.selectedEntity !== ""
               ? <div className="w-full">
                   <CreateComponent />
                   <div className="mt-2" />
                   <ComponentList />
                 </div>
               : React.string("Entity not selected")}
          </div>
          <ModalContainer />
        </div>
      </Modal_Context.Provider>
    </App_Context.Provider>
  </Editor_Context.Provider>;
};
