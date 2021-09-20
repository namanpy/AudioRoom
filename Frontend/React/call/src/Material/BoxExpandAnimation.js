import React, { createRef } from "react";
import "./css/styles.css";

export default class BoxExpandAnimation extends React.Component {

    constructor (props) {
        super();
        this.animatedDiv = createRef(); 
    }

    componentDidMount() {

        setTimeout( () => { this.animatedDiv.current.style.width = "100%"}, 1);

    }

    render() {

        return (
            <div class="material-animated-div" ref={this.animatedDiv}>
                <div class="material-animated-div-child">

                    {this.props.children}

                </div>

            </div>


        );


    }


}