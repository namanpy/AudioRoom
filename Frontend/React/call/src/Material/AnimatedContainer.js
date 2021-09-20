import React, { createRef } from "react";
import "./css/styles.css";

export default class AnimatedContainer extends React.Component {

    constructor () {
        super();

        this.inputContainer = createRef();

        this.state = {

        }
        
    }

    componentDidMount() {
        
    }

    componentDidUpdate(prevprops) {

        console.log(prevprops.close, this.props.close)
        if(prevprops.close === false && this.props.close == true) {
            this.inputContainer.current.style.height = "0";
            setTimeout(()=>{
               if(this.props.onfinish) {
                    this.props.onfinish();
               }
            }, 400);
        }


    }


    render() {

        return (
            <div>
                <div class="input-container" ref={this.inputContainer}>
                
                    {this.props.children}

                </div>

            </div>
            
        );


    }


}