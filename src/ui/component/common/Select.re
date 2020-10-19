type selectOption = {
  value: string,
  text: string,
  disabled: bool,
};

[@react.component]
let make =
    (
      ~options: Belt.List.t(selectOption), 
      ~value, 
      ~onChange, 
      ~label: option(React.element)=?,
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
    
    <select
      className={"text-black w-full focus:outline-lightblue " ++ inputClassName}
      onChange={event => onChange(ReactEvent.Form.target(event)##value)}
      value
      id
    >
      <option />
      {Belt.List.mapWithIndex(options, (index, option) =>
         <option
           key={Belt.Int.toString(index)}
           value={option.value}
           disabled={option.disabled}>
           {React.string(option.text)}
         </option>
       )
       ->Array.of_list
       ->React.array}
    </select>
  </div>;
};
