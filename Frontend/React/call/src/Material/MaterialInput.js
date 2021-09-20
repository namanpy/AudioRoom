import React, { createRef } from "react"
import "./css/styles.css"
export default class MaterialInputBox extends React.Component
{

constructor(props) {
    super();
    this.state = {
        focused  : false,
        inputValue : ""
    }
    this.materialLabel = createRef();
}

transformLabel() {
    console.log(this.materialLabel);
    if(!this.state.focused) {
        this.materialLabel.current.style.top= "0%";
        this.materialLabel.current.style.transform = "translateY(0%)";
        this.materialLabel.current.style.fontSize = "0.75em";
        this.setState({ focused : true });
    } else {
        if(this.state.inputValue == "") {
            this.materialLabel.current.style.top= "50%";
            this.materialLabel.current.style.transform = "translateY(-50%)";
            this.materialLabel.current.style.fontSize = "1em";
            this.setState({ focused : false }); 
        }
    }
}

onChange(e) {

    console.log(this.state.inputValue);
    this.setState( { inputValue  : e.target.value } );
    if(this.props.onChange) {
        this.props.onChange(e);
    }

}

render () {


    return (

        <div class="material-input" onFocus={() =>  this.transformLabel() }  onBlur={ () => this.transformLabel() }>
            <label class="material-label"   ref={this.materialLabel} for={this.props.name}>{this.props.label}</label>
            <input class="material-box"  onChange={(e) => this.onChange(e) } type="text" name={this.props.name}/>
            {/* value={this.state.username} onChange={ (e) => { this.setState( { username  : e.target.value } ) } } */}
        </div>

    );
}

}