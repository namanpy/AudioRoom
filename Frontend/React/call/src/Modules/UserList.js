import React, { createRef } from "react";
import "./userlist.css";
import BackIcon from "../Icons/back.svg";
import UserIcon from "../Icons/user.svg";
import MenuLinesIcon from "../Icons/menulines.svg";


export default class UserList extends React.Component {

    constructor (props) {
        super(props);
        this.container = createRef();
        let usrs = (props.users ? this.props.users : []);
        this.state = {
                users : usrs,
                closed : false,
                userBgColors : ["#3498db", "#9b59b6", "#e74c3c", "#e67e22", "#f1c40f", "#2ecc71"]
        }

    }
    componentDidMount() {

        if(this.state.closed == true) {

            console.log("closed????");
            this.container.current.classList.add("hide-container");
        } else {

            this.container.current.classList.remove("hide-container");


        }


    }

    componentDidUpdate(prevProps, prevState, snapshot) {

        console.log( this.props, this.props.users, " Numunnnnn")
        // if(this.props.users > 0) {         
        //     this.setState({ users : this.props.users });
        // }
        if(this.state.closed == true) {

            this.container.current.classList.add("hide-container");
        } else {
            this.container.current.classList.remove("hide-container");

        }

    }
     getRandomInt(min, max) {
        min = Math.ceil(min);
        max = Math.floor(max);
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }

    render() {

        return (
              
                    <div class = "user-list-container" ref={this.container}>
                            <img class="user-list-container-open-button" onClick={ () => { this.setState({ closed : false }) }}  src={MenuLinesIcon}></img>

                        <div class="user-list-container-header">

                            <img class="user-list-container-header-button" onClick={ () => { this.setState({ closed : true }) }}  src={BackIcon}></img>
                        </div>
                        <dl class="user-list-container-list"> 

                            <div> <dt  class="user-list-item"> <div class="user-list-item-back"></div><div class="user-icon" style={ { backgroundColor : this.state.userBgColors[this.getRandomInt(0, this.state.userBgColors.length -1)] } } ><img  src={UserIcon}  class="user-icon-image" ></img></div>   <div class="user-list-name"> <div style={ { display : "inline-block", flex : 1, height : "auto" } }> {this.props.srcUser} </div></div>  </dt> <br></br></div>

                            { this.props.users.map( (data, id) => {
                                    console.log("oof tried");
                                    return <div> <dt id={id} class="user-list-item"> <div class="user-list-item-back"></div><div class="user-icon" style={ { backgroundColor : this.state.userBgColors[this.getRandomInt(0, this.state.userBgColors.length -1)] } } ><img  src={UserIcon}  class="user-icon-image" ></img></div>   <div class="user-list-name"> <div style={ { display : "inline-block", flex : 1, height : "auto" } }> {data.name} </div></div>  </dt> <br></br></div>
                                
                                })
                            }

                        </dl>

                    </div>

                    // {/* <div class="mobile-user-list-container" >
                    //     <div class="user-list-container-header">
                    //         <img class="user-list-container-header-button" src={BackIcon}></img>
                    //     </div>
                    //     <dl class="user-list-container-list"> 
                    //         <dt class="user-list-item"> <div class="user-icon"><img  src={UserIcon}  class="user-icon-image" ></img></div>   <div class="user-list-name">Naman</div>  </dt>
                    //         <dt class="user-list-item"> <div class="user-icon"><img  src={UserIcon}  class="user-icon-image" ></img></div>   <div class="user-list-name">Naman</div>  </dt>
                    //     </dl>
                        
                    // </div>  */}

        );


    }


}