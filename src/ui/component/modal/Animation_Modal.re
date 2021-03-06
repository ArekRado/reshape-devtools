open Reshape;

let wrapModeToString = (wrapMode: Type.wrapMode) =>
  switch (wrapMode) {
  | Type.Once => "Once"
  | Type.Loop => "Loop"
  | Type.PingPong => "PingPong"
  | Type.ClampForever => "ClampForever"
  };

let timingFunctionToString = (timingFunction: Type.timingFunction) =>
  switch (timingFunction) {
  | Type.Linear => "Linear"
  | Type.EaseInQuad => "EaseInQuad"
  | Type.EaseOutQuad => "EaseOutQuad"
  | Type.EaseInOutQuad => "EaseInOutQuad"
  | Type.EaseInCubic => "EaseInCubic"
  | Type.EaseOutCubic => "EaseOutCubic"
  | Type.EaseInOutCubic => "EaseInOutCubic"
  | Type.EaseInQuart => "EaseInQuart"
  | Type.EaseOutQuart => "EaseOutQuart"
  | Type.EaseInOutQuart => "EaseInOutQuart"
  | Type.EaseInQuint => "EaseInQuint"
  | Type.EaseOutQuint => "EaseOutQuint"
  | Type.EaseInOutQuint => "EaseInOutQuint"
  | Type.CubicBezier(_, _, _, _) => "CubicBezier"
  };

let componentList: Belt.List.t(Select.selectOption) = [
  {value: "FieldFloat", text: "Field Float", disabled: false},
  {value: "FieldVector", text: "Field Vector", disabled: false},
  {
    value: "TransformLocalPosition",
    text: "Transform.localPosition",
    disabled: false,
  },
];

let parseFloat = s => switch (Belt.Float.fromString(s)) {
| Some(value) => value
| None => 0.0
};

let componentEntitiesList =
    (appState: Type.state, component: Type.animatedComponent)
    : Belt.List.t(Select.selectOption) => {
  let selectOption =
    switch (component) {
    | FieldFloat(_) =>
      Belt.Map.String.mapWithKey(appState.fieldFloat, (key, _) =>
        (
          {
            value: key,
            text: Uuid_Util.humanFriendlyEntity(key),
            disabled: false,
          }: Select.selectOption
        )
      )
    | FieldVector(_) =>
      Belt.Map.String.mapWithKey(appState.fieldVector, (key, _) =>
        (
          {
            value: key,
            text: Uuid_Util.humanFriendlyEntity(key),
            disabled: false,
          }: Select.selectOption
        )
      )
    | TransformLocalPosition(_) =>
      Belt.Map.String.mapWithKey(appState.transform, (key, _) =>
        (
          {
            value: key,
            text: Uuid_Util.humanFriendlyEntity(key),
            disabled: false,
          }: Select.selectOption
        )
      )
    // | _ => {
    //   Js.log("Wrong switch option");
    //   Belt.Map.String.empty
    // }
    };

  selectOption->Belt.Map.String.valuesToArray->Belt.List.fromArray;
};

let getComponentEntity = (component: Type.animatedComponent): string =>
  switch (component) {
  | FieldFloat(entity, _) => entity
  | FieldVector(entity, _) => entity
  | TransformLocalPosition(entity) => entity
  };

let mapTextToComponent = (component: string): Type.animatedComponent =>
  switch (component) {
  | "FieldFloat" => FieldFloat("", "")
  | "FieldVector" => FieldVector("", "")
  | "TransformLocalPosition" => TransformLocalPosition("")
  | _ =>
    Js.log("Wrong switch option");
    FieldFloat("", "");
  };

let mapComponentToText = (component: Type.animatedComponent): string =>
  switch (component) {
  | FieldFloat(_) => "FieldFloat"
  | FieldVector(_) => "FieldVector"
  | TransformLocalPosition(_) => "TransformLocalPosition"
  };

let msToTime = ms => {
  let seconds = mod_float(ms /. 1000.0, 60.0);
  let minutes = mod_float(ms /. (1000.0 *. 60.0), 60.0);

  let formattedMinutes =
    minutes < 10.0
      ? "0" ++ Js.Float.toFixed(minutes) : Js.Float.toFixed(minutes);

  let formattedSeconds =
    seconds < 10.0
      ? "0" ++ Js.Float.toFixed(seconds) : Js.Float.toFixed(seconds);

  formattedMinutes ++ ":" ++ formattedSeconds;
};

[@react.component]
let make = (~name: Type.entity) => {
  let (appState, appDispatch) = React.useContext(App_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);
  let (editorState, _) = React.useContext(Editor_Context.context);

  let (keyframeIndex, setKeyframeIndex) = React.useState(() => (-1));

  let animation =
    Animation_Component.get(
      ~state=appState,
      ~name,
      ~entity=editorState.selectedEntity,
    );

  let modalBody =
    switch (animation) {
    | None => React.string("Animation doesn't exist")
    | Some(animation) =>
      let animationLength =
        Belt.List.reduce(animation.keyframes, 0.0, (sum, keyframe) =>
          keyframe.duration +. sum
        );

      let selectedKeyframe =
        Belt.List.get(animation.keyframes, keyframeIndex);

      let setKeyframeDuration = (keyframe: Reshape.Type.keyframe, event) =>
        appDispatch(
          App_Context.SetAnimationKeyframe(
            editorState.selectedEntity,
            name,
            keyframeIndex,
            {...keyframe, duration: parseFloat(ReactEvent.Form.target(event)##value)},
          ),
        );

      let setKeyframeValueRange = (keyframe, valueRange) =>
        appDispatch(
          App_Context.SetAnimationKeyframe(
            editorState.selectedEntity,
            name,
            keyframeIndex,
            {...keyframe, valueRange},
          ),
        );

      let setIsPlaying = isPlaying =>
        appDispatch(
          App_Context.SetAnimation(
            editorState.selectedEntity,
            name,
            {...animation, isPlaying},
          ),
        );

      let setCurrentTime = event =>
        appDispatch(
          App_Context.SetAnimation(
            editorState.selectedEntity,
            name,
            {...animation, currentTime: parseFloat(ReactEvent.Form.target(event)##value)},
          ),
        );

      let addKeyframe = () => {
        appDispatch(
          App_Context.SetAnimationKeyframes(
            editorState.selectedEntity,
            name,
            Belt.List.add(
              animation.keyframes,
              Animation_Component.emptyKeyframe(10.0),
            ),
          ),
        );

        setKeyframeIndex(_ => Belt.List.length(animation.keyframes));
      };
      
      let setAnimationComponent =
          (~component, ~componentEntity, ~animationName) => {
        let animatedComponent =
          switch (component) {
          | Type.FieldFloat(_, name) =>
            Type.FieldFloat(componentEntity, name)
          | Type.FieldVector(_, name) =>
            Type.FieldVector(componentEntity, name)
          | Type.TransformLocalPosition(_) =>
            Type.TransformLocalPosition(componentEntity)
          };

        appDispatch(
          App_Context.SetAnimationComponent(
            editorState.selectedEntity,
            animationName,
            animatedComponent,
          ),
        );

        ();
      };

      <>
        <div className="grid grid-cols-12 gap-1 my-1">
          <div className="col-span-6 grid grid-cols-12 gap-1">
            <Input 
              containerClassName="col-span-12 grid grid-cols-12"
              labelClassName="col-span-4"
              inputClassName="col-span-8"
              label={React.string("name")}
              value=name 
              onChange={_ => {()}} 
              disabled=true 
            />

            <Checkbox 
              containerClassName="col-span-12 grid grid-cols-12"
              labelClassName="col-span-4"
              inputClassName="col-span-8"
              label={React.string("isPlaying")}
              value={animation.isPlaying}
              onChange={_ => {setIsPlaying(!animation.isPlaying)}}
            />

            <Input 
              containerClassName="col-span-12 grid grid-cols-12"
              labelClassName="col-span-4"
              inputClassName="col-span-8"
              label={React.string("currentTime")}
              type_="number"
              value={Belt.Float.toString(animation.currentTime)}
              onChange={setCurrentTime}
              max={Belt.Float.toString(animationLength)}
              min="0"
            />

            <div className="col-span-4"> {React.string("component")} </div>
            <div className="col-span-4">
              <Select
                options=componentList
                value={mapComponentToText(animation.component)}
                onChange={component =>
                  setAnimationComponent(
                    ~component=mapTextToComponent(component),
                    ~componentEntity="",
                    ~animationName=name,
                  )
                }
              />
            </div>
            <div className="col-span-4">
              <Select
                options={componentEntitiesList(appState, animation.component)}
                value={getComponentEntity(animation.component)}
                onChange={componentEntity =>
                  setAnimationComponent(
                    ~component=animation.component,
                    ~componentEntity,
                    ~animationName=name,
                  )
                }
              />
            </div>

            <div className="col-span-4"> {React.string("isFinished")} </div>
            <div className="col-span-8">
              {React.string(animation.isFinished ? "true" : "false")}
            </div>

            <div className="col-span-4"> {React.string("wrapMode")} </div>
            <div className="col-span-8">
              {React.string(wrapModeToString(animation.wrapMode))}
            </div>
          </div>
          <div className="col-span-6 grid grid-cols-12 gap-1 my-1">
            <Button className="col-span-12" onClick={_ => {addKeyframe()}}>
              {React.string("Add keyframe")}
            </Button>
            {switch (selectedKeyframe) {
             | None => React.null
             | Some(keyframe) =>
                <>
                 <div className="col-span-4">
                   {React.string("duration")}
                 </div>
                 <div className="col-span-8">
                   <Input
                     value={Belt.Float.toString(keyframe.duration)}
                     onChange={setKeyframeDuration(keyframe)}
                   />
                 </div>
                 <div className="col-span-4">
                   {React.string("timingFunction")}
                 </div>
                 <div className="col-span-8"> {React.string("Linear")} </div>
                 <div className="col-span-4">
                   {React.string("valueRange")}
                 </div>
                 <div className="col-span-8">
                   {switch (keyframe.valueRange) {
                    | Float(value) => 
                      <Vector 
                        value 
                        onChange={value => {setKeyframeValueRange(keyframe, Float(value))}} 
                      />
                    | Vector((v1, v2)) => 
                      <>
                        <Vector 
                          value={v1} onChange={value => {
                          setKeyframeValueRange(keyframe, Vector((value, v2)));
                        }} />
                        <Vector 
                          value={v2} onChange={value => {
                          setKeyframeValueRange(keyframe, Vector((v1, value)));
                        }} />
                      </>
                    }}
                 </div>
               </>
             }}
          </div>
        </div>
        <div
          className="flex h-20 bg-gray-700 bg-opacity-75 relative overflow-hidden">
          {Belt.List.mapWithIndex(
             animation.keyframes, (index, keyframe: Type.keyframe) =>
             <button
               key={Belt.Int.toString(index)}
               className={
                 (index === keyframeIndex ? "border-white" : "border-black") 
                 ++ " flex flex-wrap items-center justify-center border overflow-hidden"
                }
               style={ReactDOMRe.Style.make(
                 ~flex=
                   Belt.Float.toString(
                     keyframe.duration *. animationLength /. 100.0,
                   ),
                 (),
               )}
               onClick={_ => {setKeyframeIndex(_ => index)}}
              >
               {switch (keyframe.valueRange) {
                | Float((from, to_)) =>
                  <>
                    {React.string(
                       timingFunctionToString(keyframe.timingFunction),
                     )}
                    <br />
                    {React.string(Belt.Float.toString(from))}
                    {React.string(" - ")}
                    {React.string(Belt.Float.toString(to_))}
                  </>
                | Vector((from, to_)) =>
                  <>
                    {React.string(
                       timingFunctionToString(keyframe.timingFunction),
                     )}
                    <br />
                    <Vector value=from onChange={_ => {()}} />
                    {React.string(" - ")}
                    <Vector value=to_ onChange={_ => {()}} />
                  </>
                }}
             </button>
           )
           ->Array.of_list
           ->React.array}
          <div
            className="absolute w-full h-full flex flex-col justify-between pointer-events-none"
            style={ReactDOMRe.Style.make(
              ~transform=
                "translate("
                ++ Belt.Float.toString(
                     animation.currentTime *. 100.0 /. animationLength,
                   )
                ++ "%)",
              (),
            )}>
            <div className="ml-1 overflow-hidden w-10">
              {React.string(msToTime(animation.currentTime))}
            </div>
            <div className="absolute border-l border-red-500 h-full" />
          </div>
        </div>
        // <div className="ml-1 overflow-hidden w-10">{React.string(Js.Float.toFixedWithPrecision(animation.value, ~digits=2))}</div>
      </>;
    };

  <Modal
    id="animationModal"
    render={(modal: Modal_Context.modal) => modalBody}
  />;
};
