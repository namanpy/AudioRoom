import React, { createRef } from "react";
import "./controlpanel.css";
import MuteIcon from "../Icons/mute.svg";
import ScreenShareIcon from "../Icons/monitor.svg";
import Menu from "../Icons/menu.svg";
import EndCall from "../Icons/callend.svg";

const NoAudioIcon = `data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/PjxzdmcgZGF0YS1uYW1lPSJMYXllciAxIiBpZD0iTGF5ZXJfMSIgdmlld0JveD0iMCAwIDQ4IDQ4IiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciPjx0aXRsZS8+PHBhdGggZD0iTTI0LDMzYTgsOCwwLDAsMCw4LThWMTguODNsOC40MS04LjQxYTIsMiwwLDAsMC0yLjgzLTIuODNMMzIsMTMuMTdWOUE4LDgsMCwwLDAsMTYsOVYyNWE3Ljk0LDcuOTQsMCwwLDAsLjc3LDMuNGwtMS40OSwxLjQ5QTkuOTMsOS45MywwLDAsMSwxNCwyNWEyLDIsMCwwLDAtNCwwLDEzLjkyLDEzLjkyLDAsMCwwLDIuMzgsNy43OUw3LjU5LDM3LjU5YTIsMiwwLDEsMCwyLjgzLDIuODNsNC42NS00LjY1QTEzLjk0LDEzLjk0LDAsMCwwLDIyLDM4Ljg0VjQzSDIxYTIsMiwwLDAsMCwwLDRoNmEyLDIsMCwwLDAsMC00SDI2VjM4Ljg0QTE0LDE0LDAsMCwwLDM4LDI1YTIsMiwwLDAsMC00LDAsMTAsMTAsMCwwLDEtMTYuMDksNy45MmwxLjQzLTEuNDNBOCw4LDAsMCwwLDI0LDMzWm00LThhNCw0LDAsMCwxLTQsNCw0LDQsMCwwLDEtMS43NS0uNDJMMjgsMjIuODNabS04LDBWOWE0LDQsMCwwLDEsOCwwdjguMTdsLTgsOEMyMCwyNS4xLDIwLDI1LjA1LDIwLDI1WiIvPjwvc3ZnPg==`;

const NoVideoIcon = `data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/PjxzdmcgZmlsbD0ibm9uZSIgaGVpZ2h0PSIyNCIgc3Ryb2tlPSIjMDAwIiBzdHJva2UtbGluZWNhcD0icm91bmQiIHN0cm9rZS1saW5lam9pbj0icm91bmQiIHN0cm9rZS13aWR0aD0iMiIgdmlld0JveD0iMCAwIDI0IDI0IiB3aWR0aD0iMjQiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PHBhdGggZD0iTTE2IDE2djFhMiAyIDAgMCAxLTIgMkgzYTIgMiAwIDAgMS0yLTJWN2EyIDIgMCAwIDEgMi0yaDJtNS42NiAwSDE0YTIgMiAwIDAgMSAyIDJ2My4zNGwxIDFMMjMgN3YxMCIvPjxsaW5lIHgxPSIxIiB4Mj0iMjMiIHkxPSIxIiB5Mj0iMjMiLz48L3N2Zz4=`
const VideoIcon = `data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/Pjxzdmcgdmlld0JveD0iMCAwIDMyIDMyIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciPjx0aXRsZS8+PGcgZGF0YS1uYW1lPSJWaWRlbyBDYW1lcmEiIGlkPSJWaWRlb19DYW1lcmEiPjxwYXRoIGQ9Ik0xOC44Nyw3SDUuMTNBMy4xMywzLjEzLDAsMCwwLDIsMTAuMTNWMjEuODdBMy4xMywzLjEzLDAsMCwwLDUuMTMsMjVIMTguODdBMy4xMywzLjEzLDAsMCwwLDIyLDIxLjg3VjEwLjEzQTMuMTMsMy4xMywwLDAsMCwxOC44Nyw3Wk0yMCwyMS44N0ExLjEzLDEuMTMsMCwwLDEsMTguODcsMjNINS4xM0ExLjEzLDEuMTMsMCwwLDEsNCwyMS44N1YxMC4xM0ExLjEzLDEuMTMsMCwwLDEsNS4xMyw5SDE4Ljg3QTEuMTMsMS4xMywwLDAsMSwyMCwxMC4xM1oiLz48cGF0aCBkPSJNMjksOS4wN2ExLjM1LDEuMzUsMCwwLDAtMS41My40N2wtNC4yLDUuMjFhMS41MSwxLjUxLDAsMCwwLDAsMS43NGw0LjE1LDUuODZhMS4zOSwxLjM5LDAsMCwwLDEuMTcuNjUsMS4zNywxLjM3LDAsMCwwLC4zOS0uMDYsMS40NCwxLjQ0LDAsMCwwLDEtMS40VjEwLjQ2QTEuNDUsMS40NSwwLDAsMCwyOSw5LjA3Wk0yOCwxOS43MmwtMi44Ny00LjA1TDI4LDEyLjFaIi8+PC9nPjwvc3ZnPg==`

const StopScreenShareIcon = `data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/PjwhRE9DVFlQRSBzdmcgIFBVQkxJQyAnLS8vVzNDLy9EVEQgU1ZHIDEuMS8vRU4nICAnaHR0cDovL3d3dy53My5vcmcvR3JhcGhpY3MvU1ZHLzEuMS9EVEQvc3ZnMTEuZHRkJz48c3ZnIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAwIDY0IDY0IiBoZWlnaHQ9IjY0cHgiIGlkPSJTVE9QIiB2ZXJzaW9uPSIxLjEiIHZpZXdCb3g9IjAgMCA2NCA2NCIgd2lkdGg9IjY0cHgiIHhtbDpzcGFjZT0icHJlc2VydmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgeG1sbnM6eGxpbms9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkveGxpbmsiPjxwYXRoIGQ9Ik0zMiw1YzE0Ljg4OCwwLDI3LDEyLjExMiwyNywyN1M0Ni44ODgsNTksMzIsNTlTNSw0Ni44ODgsNSwzMlMxNy4xMTIsNSwzMiw1IE0zMiwyQzE1LjQzMywyLDIsMTUuNDMyLDIsMzIgIGMwLDE2LjU2OCwxMy40MzMsMzAsMzAsMzBzMzAtMTMuNDMyLDMwLTMwQzYyLDE1LjQzMiw0OC41NjcsMiwzMiwyTDMyLDJ6Ii8+PHBhdGggZD0iTTQxLjQzOCwxNy41NjNIMjIuNTYzYy0yLjc1LDAtNSwyLjI1LTUsNXYxOC44NzVjMCwyLjc1LDIuMjUsNSw1LDVoMTguODc1YzIuNzUsMCw1LTIuMjUsNS01VjIyLjU2MyAgQzQ2LjQzOCwxOS44MTMsNDQuMTg4LDE3LjU2Myw0MS40MzgsMTcuNTYzeiBNNDMuNDM4LDQwLjQzOGMwLDEuNjUtMS4zNSwzLTMsM0gyMy41NjNjLTEuNjUsMC0zLTEuMzUtMy0zVjIzLjU2M2MwLTEuNjUsMS4zNS0zLDMtMyAgaDE2Ljg3NWMxLjY1LDAsMywxLjM1LDMsM1Y0MC40Mzh6Ii8+PC9zdmc+`
export default class ControlPanel extends React.Component {

    constructor (props) {
        super();
        this.state = {

            screenShare  : false,
            video : false,
            audio : false
        };
    }

    toggleScreenShare() {

        if(!this.state.screenShare) {

            this.props.onScreenShare();

            this.setState({ screenShare : !this.state.screenShare })

        } else {

            this.props.onScreenShareStop();

            this.setState({ screenShare : !this.state.screenShare })

        }

    }

    toggleVideo() {

        this.setState({ video : !this.state.video });
        this.props.onToggleVideo();

    }

    toggleAudio() {

        this.setState({ audio : !this.state.audio });
        this.props.onToggleAudio();

    }

    render() {

        return (
                

                <div class = "media-controls">
                    
                    <a onClick={ () => this.toggleAudio() }> <img class="control-icon" src={this.state.audio ?   MuteIcon : NoAudioIcon} ></img> </a>
                    <a onClick={ () => this.toggleVideo() }> <img class="control-icon" src={ this.state.video ? VideoIcon : NoVideoIcon} ></img> </a>
                    <a onClick={ () => this.toggleScreenShare() } > <img class="control-icon" src={ this.state.screenShare ? StopScreenShareIcon :  ScreenShareIcon } ></img> </a>
                    <a href=""> <img class="control-icon" src={Menu} ></img> </a>
                    <a href=""> <img class="control-icon" src={EndCall} ></img> </a>

                    <div class="video-src">  {this.props.children} </div>
                </div>

        );


    }


}