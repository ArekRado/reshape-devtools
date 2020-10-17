[@react.component]
let make =
    (
      ~value: bool,
      ~onChange,
      ~disabled: option(bool)=?,
      ~label: option(React.element)=?,
      ~className="",
    ) => {
  <label>

      {switch (label) {
       | Some(label) => label
       | None => React.null
       }}
      <input
        className={className ++ " text-black w-full"}
        type_="checkbox"
        checked={value}
        onChange
        disabled={
          switch (disabled) {
            | Some(disabled) => disabled
            | None => false
            }
        }
      />
    </label>;
};
