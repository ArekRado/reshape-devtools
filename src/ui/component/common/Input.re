[@react.component]
let make =
    (
      ~type_="text",
      ~value,
      ~onChange,
      ~disabled: option(bool)=?,
      ~label: option(React.element)=?,
      ~max: option(string)=?,
      ~min: option(string)=?,
      ~containerClassName="",
      ~labelClassName="",
      ~inputClassName="",
      ~id="",
    ) => {
    <div className={containerClassName}>
      {switch (label) {
      | Some(label) => <label htmlFor={id} className={labelClassName}>{label}</label>
      | None => React.null
      }}
      <input
        className={"text-black w-full focus:outline-lightblue " ++ inputClassName}
        type_
        value
        onChange
        id
        disabled={
          switch (disabled) {
            | Some(disabled) => disabled
            | None => false
            }
        }
        max={
          switch (max) {
            | Some(max) => max;
            | None => ""
            }
        }
        min={
          switch (min) {
            | Some(min) => min;
            | None => ""
            }
        }
      />
    </div>;
    // className={btnSize ++ " border border-gray-600 bg-gray-800 hover:bg-gray-700"}
    // onClick={onClick}
};
