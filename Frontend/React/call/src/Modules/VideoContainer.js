import React, {useEffect, useState} from "react";


function getColumnValues(x) {

    if (x.matches) { 
        return 2;
    } else {
        return 3;
    }
}

function useMaxColumns() {
    var [columns, setcolumns] = useState(0);
    useEffect(()=>{
        var x = window.matchMedia("(max-width: 768px)")

        setcolumns(getColumnValues(x));

        x.addEventListener("change", (x) => {
            
            setcolumns(getColumnValues(x));
        });
    }, []);
    return columns;
}

export default function VideoContainer(props) {

    var columns = useMaxColumns();


    return (
    <div class="remote-vid-container">
    {props.users.map((usr, idx) => {


        if(  (props.users.length - 1 == idx)  && (idx + 1) % columns == 1) {
            let a = <div class="remote-frame-end" ref={(ref) => { usr.ref = ref; } }>
                <video class="remotevideosrc" key={usr.name} ref={ ref => {  if(ref) { ref.srcObject = usr.stream;  } }} autoPlay playsInline autoplay="autoplay" ></video>
            </div>;
            return a;
        } else {
            let a = <div class="remote-frame" ref={(ref) => { usr.ref = ref; } }>
                <video class="remotevideosrc" key={usr.name} ref={ ref => {  if(ref) { ref.srcObject = usr.stream;  } }} autoPlay playsInline autoplay="autoplay" ></video>
            </div>;
            return a;

        }

    })}

    { (props.users.length  < 1)  && <p style={ { padding  : "1rem", color : "#EEEEEE" , fontSize : "2rem"} }> Invite your friends to this call, <br></br> tell them to join <b> <u>{props.room}</u> </b> </p>}
    </div>
    );

}