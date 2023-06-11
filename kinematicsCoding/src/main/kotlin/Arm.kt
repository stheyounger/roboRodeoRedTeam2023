import java.math.BigDecimal
import java.math.RoundingMode
import kotlin.math.*

class ThreeSegmentArmKinematics(
    val upperArmLengthMM: Double,
    val forearmLengthMM: Double,
    val wristLengthMM: Double) {

    val root = Point2D(0.0, 0.0)

    data class ArmState(val upperArm: Point2D, val forearm: Point2D, val wrist: Point2D) {
        fun asList(): List<Point2D> = listOf(upperArm, forearm, wrist)
        override fun toString(): String = "ArmState(upperArm: $upperArm, forearm: $forearm, wrist: $wrist)"
    }

    private fun calculateForearmEndpoint(endEffectorPositionRelativeToArmRoot: Point2D, endEffectorAngleRelativeToGroundRad: Double): Point2D {
        return endEffectorPositionRelativeToArmRoot - Point2D.pointFromVector(endEffectorAngleRelativeToGroundRad.toBigDecimal(), wristLengthMM.toBigDecimal())
    }

    private fun calculateUpperArmEndpoint(forearmEndpoint: Point2D): Point2D {
        val distanceFromRootToForearm = hypot(forearmEndpoint.x, forearmEndpoint.y).toBigDecimal().setScale(4, RoundingMode.DOWN).toDouble()

        val angleOppositeForearmRad = RandomMath.angleRadiansFromSideLengths(oppositeSide = forearmLengthMM, upperArmLengthMM, distanceFromRootToForearm)

        val angleFromRootToForearmEndpointRad = root.angleToPointRad(forearmEndpoint)

        val angleFromRootToUpperArmRad = angleOppositeForearmRad + angleFromRootToForearmEndpointRad

        return Point2D.pointFromVector(angleFromRootToUpperArmRad.toBigDecimal(), upperArmLengthMM.toBigDecimal())
    }

    fun calculateArmState(endEffectorPositionRelativeToArmRoot: Point2D, endEffectorAngleRelativeToGroundRad: Double): ArmState {
        val endEffectorPoint = returnEndEffectorPointInBounds(endEffectorPositionRelativeToArmRoot)
        val endEffectorAngleRad = returnAngleInBounds(endEffectorPoint, endEffectorAngleRelativeToGroundRad)

        val forearmEndpoint = calculateForearmEndpoint(endEffectorPoint, endEffectorAngleRad)

        val upperArmEndpoint = calculateUpperArmEndpoint(forearmEndpoint)

        return ArmState(upperArm= upperArmEndpoint, forearm= forearmEndpoint, wrist= endEffectorPoint)
    }

    fun returnEndEffectorPointInBounds(endEffectorPositionRelativeToArmRoot: Point2D): Point2D {
        val distanceToEndEffector = root.distanceTo(endEffectorPositionRelativeToArmRoot)

        val directionToEndEffector = root.angleToPointRad(endEffectorPositionRelativeToArmRoot)

        val maxArmLength = upperArmLengthMM + forearmLengthMM + wristLengthMM

        val adjustedDistanceToEndEffector = distanceToEndEffector.coerceAtMost(maxArmLength)

        return Point2D.pointFromVector(magnitude= adjustedDistanceToEndEffector.toBigDecimal(), angleRad= directionToEndEffector.toBigDecimal())
    }

    private fun returnAngleInBounds(endEffectorPositionRelativeToArmRoot: Point2D, endEffectorAngleRelativeToGroundRad: Double): Double {

        val forearmEndpoint = calculateForearmEndpoint(endEffectorPositionRelativeToArmRoot, endEffectorAngleRelativeToGroundRad)
        val lengthToForearmEndpoint = root.distanceTo(forearmEndpoint)
        val maxLengthToForearmEndpoint = upperArmLengthMM + forearmLengthMM
        return if (lengthToForearmEndpoint > maxLengthToForearmEndpoint) {

            val distanceToEndEffector = root.distanceTo(endEffectorPositionRelativeToArmRoot)

            val angleOppositeWristRad = RandomMath.angleRadiansFromSideLengths(
                oppositeSide = wristLengthMM,
                distanceToEndEffector,
                maxLengthToForearmEndpoint
            )

            val angleFromRootToEndEffectorRad = root.angleToPointRad(endEffectorPositionRelativeToArmRoot)

            val angleToForearmEndpointRad = angleFromRootToEndEffectorRad - angleOppositeWristRad

            println("angleFromRootToEndEffectorRad: $angleFromRootToEndEffectorRad")
            println("angleOppositeWristRad: $angleOppositeWristRad")
            println("angleToForearmEndpointRad: $angleToForearmEndpointRad")

            angleToForearmEndpointRad
        } else {
            endEffectorAngleRelativeToGroundRad
        }
    }
}

data class Point2D(val x: Double, val y: Double) {
    operator fun plus(other: Any): Point2D {
        other as Point2D
        return Point2D(this.x + other.x, this.y + other.y)
    }
    operator fun minus(other: Any): Point2D {
        other as Point2D
        return Point2D(this.x - other.x, this.y - other.y)
    }
    fun angleToPointRad(other: Point2D): Double {
        return atan2(other.y - this.y, other.x - this.x)
    }

    fun distanceTo(other: Point2D): Double {
        val distance = sqrt((other.y - this.y).pow(2) + (other.x - this.x).pow(2))
        return RandomMath.roundPreciseNumber(distance)
    }

    companion object {
        fun pointFromVector(angleRad: BigDecimal, magnitude: BigDecimal): Point2D {
            return Point2D((magnitude * cos(angleRad.toDouble()).toBigDecimal()).toDouble(), (magnitude * sin(angleRad.toDouble()).toBigDecimal()).toDouble())
        }
    }

    override fun toString(): String = "Point2D($x, $y)"
}

object RandomMath {
    fun angleRadiansFromSideLengths(oppositeSide: Double, otherSideA: Double, otherSideB: Double): Double {
        return acos((otherSideA.pow(2) + otherSideB.pow(2) - oppositeSide.pow(2)) / (2 * otherSideA * otherSideB))
    }
    fun roundPreciseNumber(number: Double): Double {
        return number.toBigDecimal().setScale(4, RoundingMode.DOWN).toDouble()
    }
}